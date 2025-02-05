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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;


};
