#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class ATankPlayer;

UCLASS()
class TOONTANKS_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ABaseHUD();

protected:
	ATankPlayer* PlayerPawn;

	virtual void BeginPlay() override;

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

	UFUNCTION(BlueprintCallable)
	void BindListeners();

	UFUNCTION(BlueprintCallable)
	void UnbindListeners();
};
