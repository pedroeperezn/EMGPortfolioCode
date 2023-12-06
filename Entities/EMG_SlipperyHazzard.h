// Copyright (c) 2023 @ Runaway Concepts

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EMG_SlipperyHazzard.generated.h"

UCLASS()
class EVERYTHINGMUSTGO_API AEMG_SlipperyHazzard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEMG_SlipperyHazzard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
