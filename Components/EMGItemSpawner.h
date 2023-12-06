// Copyright (c) 2023 @ Runaway Concepts

#pragma once

#include "CoreMinimal.h"
#include "EMGUtils.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "EMGItemSpawner.generated.h"

class AShoppingItem;
class UBillboardComponent;
class UNiagaraComponent;
class UNiagaraSystem;

//Types of spawnable that spawners can handle
UENUM(BlueprintType)
enum class ETypoeOfSpawnable : uint8
{
	ShoppingItem,
	Powerup
};


UCLASS()
class EVERYTHINGMUSTGO_API AEMGItemSpawner : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AEMGItemSpawner();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Component)
	UBillboardComponent* BillboardComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ETypoeOfSpawnable TypeOfSpawnable;
	
	//Allows enabling or disabling spawning
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanSpawn;

	//Sets reference to data table from editor
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* ItemDataTable;

	//spawning rate
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SpawnRate = 1.f;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<AShoppingItem> ItemToSpawnBP;

	UFUNCTION(BlueprintCallable)
	void TimedSpawning();

	UFUNCTION(BlueprintCallable)
	void StopSpawning();

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_Spawn();

	UFUNCTION(BlueprintNativeEvent)
	void PlayHighValueAudioCue();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FTimerHandle SpawnTH;
	
	//Data structure created for storing items in player
	UPROPERTY(Replicated, BlueprintReadOnly)	
	FItemsInInventory ItemsInInventory;
};
