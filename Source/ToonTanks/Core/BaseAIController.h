// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	explicit ABaseAIController(FObjectInitializer const& ObjectInitializer);
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;


};
