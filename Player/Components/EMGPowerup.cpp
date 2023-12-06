// Fill out your copyright notice in the Description page of Project Settings.


#include "EMGPowerup.h"

#include "EMGPlayerInventory.h"
#include "FMODBlueprintStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UEMGPowerup::UEMGPowerup()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...


}

// Called when the game starts
void UEMGPowerup::BeginPlay()
{                                                                  
	Super::BeginPlay();

	PlayerRef = Cast<AEMGBasePlayer>(GetOwner());
	PowerupSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	PowerupSpawn = GetOwner()->FindComponentByClass<USceneComponent>();
}

void UEMGPowerup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	
}

// Called every frame
void UEMGPowerup::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(CurrentPowerupDuration > 0)
	{
		Server_UpdateRemainingPowerupTime();
	}
}




void UEMGPowerup::Server_UsePowerup_Implementation()
{
	//Checks which is the current power-up
	switch (CurrentPowerup)
	{
	
	case EPowerUpType::SpeedBoost:
		{
			//calls method to apply modifiers
			Server_SpeedPowerup();
			//sets timer to call method to revert powerup
			GetWorld()->GetTimerManager().SetTimer(PowerupTimerHandle,this, &UEMGPowerup::Server_RevertPowerup,SpeedPowerupDuration,false);
			//Broadcast call for playing sound cue
			OnPlaySpeedCue.Broadcast();
			break;
		}
			
	case EPowerUpType::InfiniteInventory:
		{
			InfiniteInventory();
			GetWorld()->GetTimerManager().SetTimer(PowerupTimerHandle,this, &UEMGPowerup::Server_RevertPowerup,InventoryPowerupDuration,false);
			OnPlayInfiniteCue.Broadcast();
			break;
		}

	case EPowerUpType::Shield:
		{
			Server_UseShieldPowerup();
			GetWorld()->GetTimerManager().SetTimer(PowerupTimerHandle,this, &UEMGPowerup::Server_RevertPowerup,ShieldPowerupDuration,false);
			OnPlayShieldCue.Broadcast();
			break;
		}

	//for goo and slippery trap, spawn traps, no modifier on player
	case EPowerUpType::Gooey:
		{
			if(!GooeyHazardRef)
			{
				return;
			}
			if(!PowerupSpawn)
			{
				return;
			}
			
			//set spawn parameters and spawn actor
			FVector spawningLocation = PowerupSpawn->GetComponentLocation();
			AEMG_ObstacleHazard* GooeyHazard = GetWorld()->SpawnActor<AEMG_ObstacleHazard>(GooeyHazardRef,spawningLocation,
				GetOwner()->GetActorRotation(),PowerupSpawnParams);

			//boradcast function to call sound cue
			OnPlayGooeyCue.Broadcast();
			Server_RevertPowerup();
			break;
		}

	case EPowerUpType::Slippery:
		{
			if(!SlipperyHazardRef)
			{
				return;
			}
			if(!PowerupSpawn)
			{
				return;
			}
			
			FVector spawningLocation = PowerupSpawn->GetComponentLocation();
			AEMG_SlipperyHazzard* slipperyHazard = GetWorld()->SpawnActor<AEMG_SlipperyHazzard>(SlipperyHazardRef,spawningLocation,
				GetOwner()->GetActorRotation(),PowerupSpawnParams);

			OnPlaySlipperyCue.Broadcast();
			Server_RevertPowerup();
			break;
		}
		
	}
}



void UEMGPowerup::Server_RevertPowerup_Implementation()
{
	Multicast_RevertPowerup();
}

void UEMGPowerup::Multicast_RevertPowerup_Implementation()
{
	
	switch (CurrentPowerup)
	{
		case EPowerUpType::SpeedBoost:
			RevertSpeedPowerup();
			OnStopSpeedCue.Broadcast();
			break;
		
		case EPowerUpType::InfiniteInventory:
			RevertInfiniteInventory();
			OnStopInfiniteCue.Broadcast();
			break;

		case EPowerUpType::Shield:
			OnStopShieldCue.Broadcast();
			break;
		
	}
	
	CurrentPowerup = EPowerUpType::None;
	PlayerRef->ResetPowewrupWidget();
	CurrentPowerupDuration = 0;
}

EPowerUpType UEMGPowerup::GetRandomPowerup()
{

	//return EPowerUpType::Shield;
	//Get score average in match
	float avgScore = GetPlayerScoreAverage();

	//tiers probability of powerup per tier (t1 is OP, tier3 is lame)
	float tier1 = 0.25;
	float tier2 = 0.35;
	float tier3 = 0.40;

	if(!PlayerRef)
	{
		return EPowerUpType::None;
	}
	
	if(PlayerRef->GetPlayerState()->GetScore() < avgScore * 0.5)
	{
		tier1 += 0.35;
		tier2 -= 0.10;
		tier3 -= 0.25;
	}

	if(PlayerRef->GetPlayerState()->GetScore() > avgScore * 0.5)
	{
		tier1 -= 0.1;
		tier2 -= 0.1;
		tier3 += 0.2;
	}

	float RandomTierValue = FMath::FRand();

	if(RandomTierValue < tier1)
	{
		//assign powerup between InfiniteInventory and Shield	
		int32 RandomValue = FMath::RandRange(static_cast<int32>(EPowerUpType::InfiniteInventory),static_cast<int32>(EPowerUpType::Shield));
		return static_cast<EPowerUpType>(RandomValue);
	}

	if(RandomTierValue < tier1 + tier2)
	{
		//assign SpeedBoost
		return EPowerUpType::SpeedBoost;
	}

	else
	{
		//assign between Slippery and Gooey
		int32 RandomValue = FMath::RandRange(static_cast<int32>(EPowerUpType::Slippery),static_cast<int32>(EPowerUpType::Gooey));
		return static_cast<EPowerUpType>(RandomValue);
	}
	
}

void UEMGPowerup::Server_AssignPlayerPowerup_Implementation()
{
	EPowerUpType AssignedPowerup = GetRandomPowerup();
	Multicast_AssignPlayerPowerup(AssignedPowerup);
}

void UEMGPowerup::Multicast_AssignPlayerPowerup_Implementation(EPowerUpType PowerupToAssign)
{
	CurrentPowerup = PowerupToAssign;
}

//Calls for server
void UEMGPowerup::Server_SpeedPowerup_Implementation()
{
	Multicast_SpeedPowerup();
}

//Calls for clients
void UEMGPowerup::Multicast_SpeedPowerup_Implementation()
{
	if(!PlayerRef)
	{
		return;
	}

	//Set duration of the power-up
	CurrentPowerupDuration = SpeedPowerupDuration;
	
	//Clears weight of the cart from affecting the movement
	PlayerRef->InventoryComp->bIsWeightAffectingMovement = false;
	
	//Sets new parameters for quicker movement
	PlayerRef->GetCharacterMovement()->MaxWalkSpeed = PowerUpSettings.Speed;
	PlayerRef->GetCharacterMovement()->MaxAcceleration = PowerUpSettings.Acceleration;
	const FRotator PlayerRotation = PlayerRef->GetCharacterMovement()->RotationRate; 
	PlayerRef->GetCharacterMovement()->RotationRate = FRotator(PlayerRotation.Pitch,PowerUpSettings.RotationRate,PlayerRotation.Roll);	
}


void UEMGPowerup::Server_UseShieldPowerup_Implementation()
{
	Multicast_UseShieldPowerup();
}

void UEMGPowerup::Multicast_UseShieldPowerup_Implementation()
{
	//shield only sets it's duration. Ramming ability and stealing items ability check if the player has a shield
	//if so, they don't have effect
	CurrentPowerupDuration = ShieldPowerupDuration;
}

void UEMGPowerup::RevertSpeedPowerup()
{
	PlayerRef->InventoryComp->bIsWeightAffectingMovement = true;
	
	PlayerRef->GetCharacterMovement()->MaxWalkSpeed = PlayerRef->InitialMaxSpeed;
	PlayerRef->GetCharacterMovement()->RotationRate = PlayerRef->InitialRotationRate;
	PlayerRef->GetCharacterMovement()->MaxAcceleration = PlayerRef->InitialMaxAcceleration;
}

void UEMGPowerup::InfiniteInventory()
{
	//Blocks weight from affecting player movement
	PlayerRef->InventoryComp->bIsWeightAffectingMovement = false;

	//Sets power-up duration
	CurrentPowerupDuration = InventoryPowerupDuration;
	
	//Sets movement to normal
	PlayerRef->GetCharacterMovement()->MaxAcceleration = PlayerRef->InitialMaxAcceleration;
	PlayerRef->GetCharacterMovement()->MaxWalkSpeed = PlayerRef->InitialMaxSpeed;
	PlayerRef->GetCharacterMovement()->GroundFriction = PlayerRef->InitialGroundFriction;
	PlayerRef->GetCharacterMovement()->Mass = PlayerRef->InitialMass;
	
	//Increase inventory capacity to 100
	PlayerRef->InventoryComp->InventoryCapacity = InventoryPowerUpCap;
}

void UEMGPowerup::RevertInfiniteInventory()
{
	PlayerRef->InventoryComp->InventoryCapacity = PlayerRef->InventoryComp->InitialInventoryCapacity;
	PlayerRef->InventoryComp->bIsWeightAffectingMovement = true;
	PlayerRef->InventoryComp->UpdatePlayersCartWeight();
}


bool UEMGPowerup::UsingPowerup()                                                                    
{                                                                                                   
	check(GetWorld())	                                                                            
	return GetWorld()->GetTimerManager().GetTimerRemaining(PowerupTimerHandle) > 0.f;               
}


void UEMGPowerup::Server_ToggleCanUsePowerup_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(CanUsePowerupHandle,this, &UEMGPowerup::Multicast_ToggleCanUsePowerup,1.5,false);
}

void UEMGPowerup::Multicast_ToggleCanUsePowerup_Implementation()
{
	OnCollectSoundCue.Broadcast(PlayerRef->PowerupComponent->CurrentPowerup);
	PlayerRef->bCanTriggerPowerUp = true;
}

int UEMGPowerup::GetPlayerScoreAverage()
{
	int ScoreSum = 0;
	int ScoreAverage = 0;
	
	TArray<APlayerState*> PlayerStates = GetWorld()->GetGameState()->PlayerArray;
	
	if(PlayerStates.Num() == 0)
	{
		return 0;
	}

	for (auto PlayerState : PlayerStates)
	{
		ScoreSum += PlayerState->GetScore();
	}

	ScoreAverage = ScoreSum/PlayerStates.Num();

	return ScoreAverage;
}

void UEMGPowerup::Server_UpdateRemainingPowerupTime_Implementation()
{
	float remainingTime = GetWorld()->GetTimerManager().GetTimerRemaining(PowerupTimerHandle);
	PowerupPercent = remainingTime/CurrentPowerupDuration;
	Client_UpdateRemainingPowerupTime(PowerupPercent);
}

void UEMGPowerup::Client_UpdateRemainingPowerupTime_Implementation(float remainingTime)
{
	PowerupPercent = remainingTime;
}

