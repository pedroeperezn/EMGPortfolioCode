﻿// Copyright (c) 2023 @ Runaway Concepts - Rafael Zagolin

#pragma once

#include "CoreMinimal.h"
#include "EMGUtils.h"
#include "EverythingMustGo/Player/Interfaces/EMGPlayerControllerInterface.h"
#include "GameFramework/PlayerController.h"


#include "EMGPlayerController.generated.h"

UCLASS()
class EVERYTHINGMUSTGO_API AEMGPlayerController : public APlayerController, public IEMGPlayerControllerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEMGPlayerController();

	UPROPERTY(BlueprintReadOnly, Replicated)
	int32 MatchTimeLeft;	
	UPROPERTY(BlueprintReadOnly, Replicated)
	FString WinnerName;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_PlayerInfo)
	FPlayerInfo PlayerInfo;

	UFUNCTION()
	void OnRep_PlayerInfo();

	UFUNCTION(BlueprintCallable)
	void AssignPlayerInfo(FPlayerInfo _PlayerInfo);

	virtual FPlayerInfo GetPlayerInfo_Implementation() override;

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_UpdateScoreRequest();
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_UpdateScoreRequest();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent)
	void BPIE_AssignPlayerInfo(FPlayerInfo _PlayerInfo);

	FTimerHandle Timer;
};
