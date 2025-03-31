#include "BTTask_Shoot.h"

#include "../Core/BaseAIController.h"
#include "../Character/TankNPC.h"

UBTTask_Shoot::UBTTask_Shoot(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Shoot");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ABaseAIController* const Controller = Cast<ABaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ATankNPC* NPC = Cast<ATankNPC>(Controller->GetPawn()))
		{
			NPC->RequestShoot();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
