#include "BTTask_PawnMoveTo.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "../Character/Tank.h"
#include "../Core/BaseAIController.h"
#include "../Attributes/AttributesComponent.h"

UBTTask_PawnMoveTo::UBTTask_PawnMoveTo(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "PawnMoveTo";
}

EBTNodeResult::Type UBTTask_PawnMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATank* NPC = Cast<ATank>(Controller->GetPawn()))
		{
			FVector LocationToMove;
			LocationToMove = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLocation"));

			NPC->AddActorLocalOffset(LocationToMove, true);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
