#include "BaseHUD.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/TankPlayer.h"
#include "../Attributes/AttributesComponent.h"

ABaseHUD::ABaseHUD()
{

}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	ATankPlayer* Player = Cast<ATankPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (Player)
	{
		Player->GetAttributesComponent()->OnHealthUpdated.AddDynamic(this, &ABaseHUD::HandleHealthUpdated);
		Player->GetSkillData().OnSkillStarted.AddDynamic(this, &ABaseHUD::HandleSkillStart);
		Player->GetSkillData().OnSkillEnded.AddDynamic(this, &ABaseHUD::HandleSkillEnd);
		Player->OnActorGotSkill.AddDynamic(this, &ABaseHUD::HandleNewSkill);

	}
}
