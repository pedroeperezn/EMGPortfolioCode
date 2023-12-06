// Copyright (c) 2023 @ Runaway Concepts


#include "EMGPlayerState.h"
#include "EverythingMustGo/Player/PlayerControllers/EMGPlayerController.h"
#include "EverythingMustGo/Player/Interfaces/EMGPlayerControllerInterface.h"
#include "Net/UnrealNetwork.h"


void AEMGPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	OnScoreUpdate_BPIE(GetScore());}


#pragma region Interface functions
/* With these interfaces function allow us to return only the necessary information and
* also it's a way to get around with casting validations from other classes.
*/

FVector AEMGPlayerState::GetControlledPawnLocation_Implementation()
{
	FVector pawnLocation;
	if(GetPawn())
	{
		pawnLocation = GetPawn()->GetActorLocation();
	}
	return pawnLocation;
}

FVector AEMGPlayerState::GetControlledPawnForwardDirection_Implementation()
{
	FVector pawnForwardDir;
	if(GetPawn())
	{
		pawnForwardDir = GetPawn()->GetActorForwardVector();
	}
	return pawnForwardDir;
}

FPlayerInfo AEMGPlayerState::GetPlayerInfo_Implementation()
{
	IEMGPlayerControllerInterface* pcInterface = Cast<IEMGPlayerControllerInterface>(GetOwner());
	if(pcInterface)
	{
		return IEMGPlayerControllerInterface::Execute_GetPlayerInfo(GetOwner());
	}	
	return FPlayerInfo();
}

void AEMGPlayerState::SetNewScore(float NewScore)
{
	//this->SetScore(NewScore);
}

#pragma endregion 

