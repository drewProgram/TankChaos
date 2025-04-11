#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "../Attributes/Passives.h"

#include "BaseHUD.generated.h"

class ATankPlayer;
class UUserWidget;

UCLASS()
class TOONTANKS_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ABaseHUD();

	UFUNCTION(BlueprintCallable)
	void UnbindListeners();

protected:
	ATankPlayer* PlayerPawn;

	FTimerHandle NotificationTimer;

	UPROPERTY(BlueprintReadWrite)
	TMap<FGameplayTag, UUserWidget*> BuffMap;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetTimerToDeleteNotification(float Duration);

	UFUNCTION(BlueprintImplementableEvent)
	void SetNotificationTextToBlank();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleHealthUpdated();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSkillStart();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSkillEnd();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleNewSkill();

	UFUNCTION(BlueprintImplementableEvent)
	void HandleSkillRemoved();

	UFUNCTION(BlueprintImplementableEvent)
	void HandlePassiveAdded(FPassive Passive);

	UFUNCTION(BlueprintImplementableEvent)
	void HandlePassiveRemoved(FPassive Passive);

	UFUNCTION(BlueprintImplementableEvent)
	void HandleWaveChanged();

	UFUNCTION(BlueprintCallable)
	void BindListeners();

	
};
