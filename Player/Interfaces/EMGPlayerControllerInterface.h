#pragma once

#include "CoreMinimal.h"
#include "EMGUtils.h"


#include "EMGPlayerControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEMGPlayerControllerInterface : public UInterface
{
public:
	GENERATED_BODY()
};


class EVERYTHINGMUSTGO_API IEMGPlayerControllerInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FPlayerInfo GetPlayerInfo();
	
};