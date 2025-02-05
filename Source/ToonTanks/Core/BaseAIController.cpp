#include "BaseAIController.h"
#include "../Character/TankNPC.h"

ABaseAIController::ABaseAIController(FObjectInitializer const& ObjectInitializer)
{

}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (ATankNPC* const NPC = Cast<ATankNPC>(InPawn))
	{
		if (UBehaviorTree* const Tree = NPC->GetBehaviorTree())
		{
			UE_LOG(LogTemp, Warning, TEXT("Possessing boss"));
			UBlackboardComponent* BB;
			UseBlackboard(Tree->BlackboardAsset, BB);
			Blackboard = BB;
			RunBehaviorTree(Tree);
		}
	}
}
