// Copyright (c) 2023 @ Runaway Concepts


#include "EMGItemSpawner.h"
#include "Components/BillboardComponent.h"
#include "TimerManager.h"
#include "EverythingMustGo/Entities/EMG_PowerupPickup.h"
#include "EverythingMustGo/Entities/ShoppingItem.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AEMGItemSpawner::AEMGItemSpawner()
{
	// Set this actor to call Tick() every frame. 
	PrimaryActorTick.bCanEverTick = true;

	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard Component"));

	if(BillboardComponent)
	{
		RootComponent = BillboardComponent;		
	}
}

// Called when the game starts or when spawned
void AEMGItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	verify(ItemDataTable != NULL);
}

// Called every frame
void AEMGItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEMGItemSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemsInInventory);
}

void AEMGItemSpawner::TimedSpawning()
{
	if(!GetWorld())
		return;
	
	//Spawn a new item after a certain amount of time
	GetWorld()->GetTimerManager().SetTimer(SpawnTH, this, &AEMGItemSpawner::Server_Spawn, SpawnRate, false);
}

void AEMGItemSpawner::StopSpawning()
{
	if(!GetWorld())
		return;

	//Clear spawner time handle
	GetWorld()->GetTimerManager().ClearTimer(SpawnTH);
}

//Broadcast when triggered in order to play sound cue
void AEMGItemSpawner::PlayHighValueAudioCue_Implementation()
{
	
}


void AEMGItemSpawner::Server_Spawn_Implementation()
{

	//If the item is a shopping item:
	if(bCanSpawn && TypeOfSpawnable == ETypoeOfSpawnable::ShoppingItem)
	{
		//Set spawn parameters
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		//Guard check for data table assigned in editor
		if(!ItemDataTable)
		{
			UE_LOG(LogTemp,Warning,TEXT("No data table loaded"));
			return;
		}
	
		//Get all rows in data table and then get a random number which will grab a row from the table
		TArray<FName> ItemsInDT = ItemDataTable->GetRowNames();
		int32 NumberOfItems = ItemsInDT.Num();
		if(NumberOfItems == 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("Empty Data Table"));
			return;
		}
		int32 RandomItemNumber = FMath::RandRange(0, NumberOfItems-1);
	
		//Create a new shopping item grabing fields from the data table
		FItemsInInventory* newItem = ItemDataTable->FindRow<FItemsInInventory>(ItemsInDT[RandomItemNumber],TEXT("Looking for random item"));
		checkf(newItem, TEXT("Error loading item from Data Table to spawn item"));
	
		ItemsInInventory = *newItem;
	
		//Create new item
		AShoppingItem* ShoppingItem = GetWorld()->SpawnActor<AShoppingItem>(ItemToSpawnBP,
			BillboardComponent->GetComponentLocation(),
			BillboardComponent->GetComponentRotation(), ActorSpawnParams);
	
		//Spawn item
		if(ShoppingItem)
		{
			ShoppingItem->Item = ItemsInInventory;
	
			ShoppingItem->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ItemsInInventory.ItemMesh);
			ShoppingItem->SetParticle(ItemsInInventory.NiagaraParticles);
			
			//set a reference to the spawner inside of the shopping item
			ShoppingItem->ItemSpawner = this;
			bCanSpawn = false;

			if(ShoppingItem->Item.ItemSize == EItemSize::Large)
			{
				PlayHighValueAudioCue();
			}
		}
	}

	//If the spawner is assigned to be for powerups
	else if(bCanSpawn && TypeOfSpawnable == ETypoeOfSpawnable::Powerup )
	{
		//Set spawn parameters
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		//create item
		AEMG_PowerupPickup* Powerup = GetWorld()->SpawnActor<AEMG_PowerupPickup>(ItemToSpawnBP,
			BillboardComponent->GetComponentLocation(),
			BillboardComponent->GetComponentRotation(), ActorSpawnParams);

			if(Powerup)
			{
				Powerup->ItemSpawner = this;
				bCanSpawn = false;
			}
	}

}






