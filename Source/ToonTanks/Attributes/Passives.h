#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Passives.generated.h"

class UAttributesComponent;

USTRUCT(BlueprintType)
struct FPassive
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGuid PassiveId;

	UPROPERTY(EditAnywhere)
	float MaxDuration;

	float Duration;

	UPROPERTY(EditAnywhere)
	float Modifier;

	UPROPERTY(EditAnywhere)
	FGameplayTag PassiveType;

	FTimerHandle TimerHandle;

	void Apply(UAttributesComponent* Ref, UWorld* WorldRef);
	void Remove(UAttributesComponent* Ref);

	bool operator==(const FPassive& Other);
};

USTRUCT(BlueprintType)
struct FPassiveStack
{
	GENERATED_BODY()

	void Add(FPassive*);
	void Remove(FPassive*);

	float GetTotalModifier();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float TotalModifier;

	TArray<FPassive*> Passives;
};