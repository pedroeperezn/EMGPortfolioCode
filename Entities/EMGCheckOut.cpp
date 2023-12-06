// Copyright (c) 2023 @ Runaway Concepts - Pedro E. Perez


#include "EMGCheckOut.h"

#include "EverythingMustGo/Player/EMGBasePlayer.h"
#include "EverythingMustGo/Player/Components/EMGPlayerInventory.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEMGCheckOut::AEMGCheckOut()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEMGCheckOut::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEMGCheckOut::InteractWithCheckOut_Implementation(ACharacter* playerCharacter)
{
	IInteractInterface::InteractWithCheckOut_Implementation(playerCharacter);


	bIsPlayerCheckingOut = true;
	//Check if i'm receiving valid pointer
	if(IsValid(playerCharacter))
	{
		//Gets the Inventory from the player and validates it
		UActorComponent* ActorComponent = playerCharacter->GetComponentByClass(UEMGPlayerInventory::StaticClass());
		UEMGPlayerInventory* playerInventory = Cast<UEMGPlayerInventory>(ActorComponent);
		
		if(IsValid(playerInventory))
		{

			playerInventory->Server_EmptyInventory();
		}
	}
		


	
	
}

// Called every frame
void AEMGCheckOut::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEMGCheckOut::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEMGCheckOut,  bIsPlayerCheckingOut);
}

//void AEMGCheckOut::ServerCheckoutSingleItem_Implementation(ACharacter* playerCharacter)
//{
	//MulticastCheckoutSingleItem(playerCharacter);

	//AEMGBasePlayer* player = Cast<AEMGBasePlayer>(playerCharacter);

	//if(!player)
	//{
	//	return;
	//}
	//
	//UEMGPlayerInventory* playerInventory = player->InventoryComp;

	//if(!playerInventory)
	//{
	//	return;
	//}

	//// deleting one single item from inventory and adding it to the player score
	//int currPlayerScore = player->GetPlayerState()->GetScore();
	//
	//if(playerInventory->PlayerInventory.Num() >= 1)
	//{
	//	int newPlayerScore = currPlayerScore + playerInventory->PlayerInventory.Top().Value;
	//	player->GetPlayerState()->SetScore(newPlayerScore);
	//	player->InventoryComp->Server_RemoveLastItem();
	//}
//}


