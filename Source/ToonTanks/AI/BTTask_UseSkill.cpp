#include "BTTask_UseSkill.h"

#include "../Core/BaseAIController.h"
#include "../Character/Tank.h"

UBTTask_UseSkill::UBTTask_UseSkill(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("UseSkill");
}

EBTNodeResult::Type UBTTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATank* NPC = Cast<ATank>(Controller->GetPawn()))
		{
			UE_LOG(LogTemp, Warning, TEXT("Will try to cast skill"));
			NPC->RequestSkillCast();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
