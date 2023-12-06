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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bCanSpawn;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* ItemDataTable;

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
	FTimerHandle SpawnTH; // spawn timer handle
	


	UPROPERTY(Replicated, BlueprintReadOnly)	
	FItemsInInventory ItemsInInventory;

	


	
};
