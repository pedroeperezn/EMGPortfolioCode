// Copyright (c) 2023 @ Runaway Concepts - Rafael Zagolin


#include "EMGPlayerController.h"

#include "EverythingMustGo/Player/EMGBasePlayer.h"
#include "EverythingMustGo/GameInstances/EMGGameInstance.h"
#include "EverythingMustGo/EMGConsoleManager.h"
#include "EverythingMustGo/Player/Components/EMGPlayerInventory.h"
#include "EMGGameMode.h"


#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

#include <imgui.h>


#define LOG(time, text) if(GEngine) \
GEngine->AddOnScreenDebugMessage(-1, time, FColor::Green, FString(text));

// Sets default values
AEMGPlayerController::AEMGPlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



// Called when the game starts or when spawned
void AEMGPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Assign server player controller reference for the game mode
	if(GetNetMode() == NM_ListenServer)
	{
		AEMGGameMode* GameModeRef = Cast<AEMGGameMode>( UGameplayStatics::GetGameMode(GetWorld()->GetAuthGameMode()));
		if(GameModeRef)
		{
			GameModeRef->ServerPlayerController = this;
		}
	}

	//GetWorld()->GetTimerManager().SetTimer(Timer, [&]() { Server_UpdateScoreRequest(); }, 0.f, false, 3.f);
	
}

// Called every frame
void AEMGPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AEMGPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	
}

void AEMGPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEMGPlayerController, MatchTimeLeft);	
	DOREPLIFETIME(AEMGPlayerController, WinnerName);	
	DOREPLIFETIME(AEMGPlayerController, PlayerInfo);	
}

void AEMGPlayerController::OnRep_PlayerInfo()
{
	// Assign info on possessed player
	BPIE_AssignPlayerInfo(PlayerInfo);
}

void AEMGPlayerController::AssignPlayerInfo(FPlayerInfo _PlayerInfo)
{
	if(HasAuthority())
		this->PlayerInfo = _PlayerInfo;
}

FPlayerInfo AEMGPlayerController::GetPlayerInfo_Implementation()
{
	return PlayerInfo;
}

void AEMGPlayerController::Server_UpdateScoreRequest_Implementation()
{
	Multicast_UpdateScoreRequest();
}

void AEMGPlayerController::Multicast_UpdateScoreRequest_Implementation()
{
	if (AEMGGameMode* GM = Cast<AEMGGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
	{
		GM->LoadPlayerScores();
	}
}