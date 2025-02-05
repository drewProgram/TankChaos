#include "BTTask_RotateToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../Character/Tank.h"
#include "../Core/BaseAIController.h"
#include "../Attributes/AttributesComponent.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "RotateToTarget";
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATank* NPC = Cast<ATank>(Controller->GetPawn()))
		{
			FVector TargetLocation;
			TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("TargetLocation"));

			FVector ToTarget = TargetLocation - NPC->GetActorLocation();
			FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
			NPC->SetActorRotation(LookAtRotation);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
