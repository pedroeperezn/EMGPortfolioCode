// Copyright (c) 2023 @ Runaway Concepts - Pedro E. Perez

#pragma once

#include "CoreMinimal.h"
#include "EverythingMustGo/Interfaces/InteractInterface.h"
#include "GameFramework/Actor.h"


#include "EMGCheckOut.generated.h"

UCLASS()
class EVERYTHINGMUSTGO_API AEMGCheckOut : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	

	AEMGCheckOut();

protected:

	virtual void BeginPlay() override;

	//Interaction Interface
	UFUNCTION(BlueprintCallable)
	virtual void InteractWithCheckOut_Implementation(ACharacter* playerCharacter) override;
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//UFUNCTION(Server,Reliable,BlueprintCallable)
	//void ServerCheckoutSingleItem(ACharacter* playerCharacter);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool bIsPlayerCheckingOut;
};
