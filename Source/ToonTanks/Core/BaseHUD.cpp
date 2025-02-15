#include "BaseHUD.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/TankPlayer.h"
#include "../Attributes/AttributesComponent.h"
#include "../Skill/SkillDataObject.h"

ABaseHUD::ABaseHUD()
{

}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	ATankPlayer* Player = Cast<ATankPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (Player)
	{
		if (Player->GetSkillDataObject())
		{
			Player->GetAttributesComponent()->OnHealthUpdated.AddDynamic(this, &ABaseHUD::HandleHealthUpdated);
			Player->GetSkillDataObject()->GetOnSkillStarted().AddDynamic(this, &ABaseHUD::HandleSkillStart);
			Player->GetSkillDataObject()->GetOnSkillEnded().AddDynamic(this, &ABaseHUD::HandleSkillEnd);
			Player->OnActorGotSkill.AddDynamic(this, &ABaseHUD::HandleNewSkill);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SkillDataObject nullptr"));
		}
	}
}
