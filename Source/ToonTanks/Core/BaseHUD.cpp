#include "BaseHUD.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/TankPlayer.h"
#include "../Attributes/AttributesComponent.h"
#include "ToonTanksGameMode.h"
#include "../Skill/SkillDataObject.h"

ABaseHUD::ABaseHUD()
{
	PlayerPawn = nullptr;
}

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<ATankPlayer>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
	if (PlayerPawn)
	{
		if (PlayerPawn->GetSkillDataObject())
		{
			PlayerPawn->GetAttributesComponent()->OnHealthUpdated.AddDynamic(this, &ABaseHUD::HandleHealthUpdated);
			PlayerPawn->GetAttributesComponent()->OnPassiveAdded.AddUObject(this, &ABaseHUD::HandlePassiveAdded);
			PlayerPawn->GetAttributesComponent()->OnPassiveRemoved.AddUObject(this, &ABaseHUD::HandlePassiveRemoved);
			PlayerPawn->OnActorGotSkill.AddDynamic(this, &ABaseHUD::HandleNewSkill);

			AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
			GameMode->OnWaveStarted.AddUObject(this, &ABaseHUD::HandleWaveChanged);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SkillDataObject nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn is nullptr"));
	}
}

void ABaseHUD::SetTimerToDeleteNotification(float Duration)
{
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&ABaseHUD::SetNotificationTextToBlank,
		Duration,
		false
	);
}

void ABaseHUD::BindListeners()
{
	PlayerPawn->GetSkillDataObject()->GetOnSkillStarted().AddDynamic(this, &ABaseHUD::HandleSkillStart);
	PlayerPawn->GetSkillDataObject()->GetOnSkillEnded().AddDynamic(this, &ABaseHUD::HandleSkillEnd);
	PlayerPawn->GetSkillDataObject()->GetOnSkillSlotRemoved().AddDynamic(this, &ABaseHUD::HandleSkillRemoved);
}

void ABaseHUD::UnbindListeners()
{
	PlayerPawn->GetSkillDataObject()->GetOnSkillStarted().RemoveDynamic(this, &ABaseHUD::HandleSkillStart);
	PlayerPawn->GetSkillDataObject()->GetOnSkillEnded().RemoveDynamic(this, &ABaseHUD::HandleSkillEnd);
	PlayerPawn->GetSkillDataObject()->GetOnSkillSlotRemoved().RemoveDynamic(this, &ABaseHUD::HandleSkillRemoved);
}
