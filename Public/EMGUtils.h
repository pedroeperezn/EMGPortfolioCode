#pragma once

#include "CoreMinimal.h"
#include "FMODEvent.h"
#include "Engine/DataTable.h"
#include "NiagaraComponent.h"


#include "EMGUtils.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMatchStarted);

class AEMGPlayerState;
class AEMGPlayerController;

UENUM(BlueprintType)
enum EItemSize
{
	NoItem,
	Small = 1,
	Medium = 2,
	Large = 3
};


UENUM(BlueprintType)
enum class ETutorials : uint8
{
	NONE = 0,
	Ram,
	Checkout,
	Powerup,
	CollectItems,
	KnockOutItems
};


USTRUCT(BlueprintType)
struct FItemsInInventory : public FTableRowBase
{

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Value;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TEnumAsByte<EItemSize> ItemSize;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMesh* ItemMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UNiagaraSystem* NiagaraParticles;
	
};

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()
	
	/* Check to see if this data is occupied */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerID")
	bool bOccupied;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerID")
	FColor PlayerColor;

	/* Info to store in the player id */
	UPROPERTY(BlueprintReadWrite, Category = "PlayerID")
	FString PlayerID;

};

USTRUCT(BlueprintType)
struct FPowerUpSettings
{
	GENERATED_BODY()
	
	/* New player speed */
	UPROPERTY(EditAnywhere,  Category = "Speed")
	float Speed;

	/* New player rotation rate */
	UPROPERTY(EditAnywhere,  Category = "Speed")
	float RotationRate;
	
	/* New player acceleration */
	UPROPERTY(EditAnywhere,  Category = "Speed")
	float Acceleration;

};

/* Stores relevant data for offscreen component logic */
USTRUCT(BlueprintType)
struct FOffscreenIndicatorData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	AEMGPlayerState* PlayerState;
	
	/* Player nick name */
	UPROPERTY(BlueprintReadWrite)
	FString PlayerID;

	// I believe this is deprecated (We might not be using it anymore.)
	/* Screen boundaries */
	UPROPERTY(BlueprintReadWrite)
	uint8 bInsideScreen : 1;

	/* Player color ID */
	UPROPERTY(BlueprintReadWrite)
	FColor PlayerColor;
	

};


USTRUCT(BlueprintType)
struct FGameModeSettings : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MatchTimeMinutes = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 StartupTimeSeconds = 5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float InitialDelay = 5;

};


USTRUCT(BlueprintType)
struct FTutorialDialogue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> Dialogues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UFMODEvent*> DialogueAudio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETutorials CurrentTutorial;
};
