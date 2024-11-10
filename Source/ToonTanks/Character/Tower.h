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

	virtual void HandleDestruction() override;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Turn(FVector Target);

private:
	UPROPERTY(VisibleAnywhere)
	ATank* Tank;

	UPROPERTY(EditAnywhere)
	float FireRange;

	FTimerHandle FireRateTimerHandle;
	float FireRate;

	bool CheckTankIsInRange();
	void CheckFireCondition();
};
