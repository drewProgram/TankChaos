// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseSkill.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API UBTTask_UseSkill : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseSkill(FObjectInitializer const& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
