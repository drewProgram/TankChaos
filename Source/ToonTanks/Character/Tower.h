// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

class ATank;

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATower();

	int32 Bory;

	virtual void HandleDestruction() override;

	virtual void Tick(float DeltaTime) override;
	void RemoveCooldown();
	
	UFUNCTION()
	void Turn(FVector Target);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleStatusApplied(FGameplayTag StatusTag, float Duration);
	void HandleStatusRemoved();

private:
	UPROPERTY(VisibleAnywhere)
	ATank* Tank;

	UFUNCTION()
	void EnableShooting();

	UPROPERTY(EditAnywhere)
	float FireRange;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(VisibleAnywhere)
	bool bCanShoot;

	bool CheckTankIsInRange();
};
