// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"

#include "BehaviorTree/BehaviorTree.h"

#include "TankNPC.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATankNPC : public ATank
{
	GENERATED_BODY()
	
public:
	ATankNPC();

	UBehaviorTree* GetBehaviorTree() const;

	virtual void Tick(float DeltaTime) override;

	FVector GetRandomLocation();

	virtual void HandleDestruction() override;

	void RequestShoot();
	void RemoveCooldown();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UWidgetComponent* HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

	class ABaseAIController* AIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UFloatingPawnMovement* MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SearchRadius;

	UPROPERTY(BlueprintReadWrite ,Category = "Movement")
	FVector LocationToMove;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(VisibleAnywhere)
	bool bCanShoot;

	UFUNCTION(BlueprintImplementableEvent)
	void HandleHealthBarUpdate();

	void FindRandomLocation();
};
