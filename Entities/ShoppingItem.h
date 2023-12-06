// Copyright (c) 2023 @ Runaway Concepts

#pragma once

#include "CoreMinimal.h"
#include "EverythingMustGo/Components/EMGItemSpawner.h"
#include "EverythingMustGo/Interfaces/InteractInterface.h"
#include "GameFramework/Actor.h"
#include "EverythingMustGo/Public/EMGUtils.h"


#include "ShoppingItem.generated.h"

class UNiagaraSystem;

UCLASS()
class EVERYTHINGMUSTGO_API AShoppingItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShoppingItem();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UNiagaraComponent* NiagaraComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FItemsInInventory Item;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AEMGItemSpawner* ItemSpawner;

	UFUNCTION(NetMulticast,Reliable)
	void SetParticle(UNiagaraSystem* _NS);

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_Change)
	UNiagaraSystem* NS;

	UFUNCTION()
	void OnRep_Change(UNiagaraSystem* _NS);
	
};


