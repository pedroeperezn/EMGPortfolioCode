// Copyright (c) 2023 @ Runaway Concepts


#include "ShoppingItem.h"

#include "Net/UnrealNetwork.h"

#define LOG(time, text) if(GEngine) \
GEngine->AddOnScreenDebugMessage(-1, time, FColor::Green, FString(text));

// Sets default values
AShoppingItem::AShoppingItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	if(NiagaraComponent)
	{
		NiagaraComponent->SetIsReplicated(true);
		//NS = NiagaraComponent->GetAsset();
	}

}

void AShoppingItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NS);
}

void AShoppingItem::OnRep_Change(UNiagaraSystem* _NS)
{
	NiagaraComponent->SetAsset(_NS);	
}

void AShoppingItem::SetParticle_Implementation(UNiagaraSystem* _NS)
{
	NiagaraComponent->SetAsset(_NS);
}


