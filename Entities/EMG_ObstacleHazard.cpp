// Copyright (c) 2023 @ Runaway Concepts


#include "EMG_ObstacleHazard.h"


// Sets default values
AEMG_ObstacleHazard::AEMG_ObstacleHazard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEMG_ObstacleHazard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEMG_ObstacleHazard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

