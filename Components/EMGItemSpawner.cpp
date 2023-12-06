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
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTH, this, &AEMGItemSpawner::Server_Spawn, SpawnRate, false);
}

void AEMGItemSpawner::StopSpawning()
{
	if(!GetWorld())
		return;

//	UE_LOG(LogTemp, Display, TEXT("StopSpawning"));

	GetWorld()->GetTimerManager().ClearTimer(SpawnTH);
}

void AEMGItemSpawner::PlayHighValueAudioCue_Implementation()
{
	
}


void AEMGItemSpawner::Server_Spawn_Implementation()
{
	if(bCanSpawn && TypeOfSpawnable == ETypoeOfSpawnable::ShoppingItem)
	{
		//UE_LOG(LogTemp, Display, TEXT("Spawning"));
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
		if(!ItemDataTable)
		{
			UE_LOG(LogTemp,Warning,TEXT("No data table loaded"));
			return;
		}
	
		TArray<FName> ItemsInDT = ItemDataTable->GetRowNames();
		int32 NumberOfItems = ItemsInDT.Num();
	
		if(NumberOfItems == 0)
		{
			UE_LOG(LogTemp,Warning,TEXT("Empty Data Table"));
			return;
		}
	
		int32 RandomItemNumber = FMath::RandRange(0, NumberOfItems-1);
	
		FItemsInInventory* newItem = ItemDataTable->FindRow<FItemsInInventory>(ItemsInDT[RandomItemNumber],TEXT("Looking for random item"));
		
	
		checkf(newItem, TEXT("Error loading item from Data Table to spawn item"));
	
		ItemsInInventory = *newItem;
	
		AShoppingItem* ShoppingItem = GetWorld()->SpawnActor<AShoppingItem>(ItemToSpawnBP,
			BillboardComponent->GetComponentLocation(),
			BillboardComponent->GetComponentRotation(), ActorSpawnParams);
	
		if(ShoppingItem)
		{
			ShoppingItem->Item = ItemsInInventory;
	
			ShoppingItem->FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(ItemsInInventory.ItemMesh);
			ShoppingItem->SetParticle(ItemsInInventory.NiagaraParticles);
			
			ShoppingItem->ItemSpawner = this;
			bCanSpawn = false;

			if(ShoppingItem->Item.ItemSize == EItemSize::Large)
			{
				PlayHighValueAudioCue();
			}
		}
	}

	else if(bCanSpawn && TypeOfSpawnable == ETypoeOfSpawnable::Powerup )
	{
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
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






