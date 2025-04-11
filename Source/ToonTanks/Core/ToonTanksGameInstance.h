#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"

#include "Configurations.h"

#include "ToonTanksGameInstance.generated.h"

UCLASS()
class TOONTANKS_API UToonTanksGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UToonTanksGameInstance();

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	FName GetCurrentGraphicsPreset() const;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInGame;

private:
	UPROPERTY(BlueprintGetter=GetCurrentGraphicsPreset)
	FName CurrentGraphicsPreset;


	UFUNCTION(BlueprintCallable)
	void ApplyGraphicSettings(FName PresetName);
};
