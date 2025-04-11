#include "ToonTanksGameInstance.h"

#include "Engine/DataTable.h"
#include "GameFramework/GameUserSettings.h"

UToonTanksGameInstance::UToonTanksGameInstance()
{
	CurrentGraphicsPreset = TEXT("Medium");
}

void UToonTanksGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Game Instance created!"));
	
	ApplyGraphicSettings(CurrentGraphicsPreset);
}

FName UToonTanksGameInstance::GetCurrentGraphicsPreset() const
{
	return CurrentGraphicsPreset;
}

void UToonTanksGameInstance::ApplyGraphicSettings(FName PresetName)
{
	static const FString Path = TEXT("/Game/Data/DT_GraphicsPresets.DT_GraphicsPresets");
	UDataTable* PresetTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *Path));

	if (!PresetTable)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get preset table."));
		return;
	}

	const FGraphicsPreset* Preset = PresetTable->FindRow<FGraphicsPreset>(PresetName, TEXT(""));
	if (!Preset) return;

	UGameUserSettings* Settings = GEngine->GetGameUserSettings();

	if (!Settings)
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get game user settings."));
		return;
	}

	CurrentGraphicsPreset = PresetName;

	Settings->SetAntiAliasingQuality(Preset->AntiAliasingQuality);
	Settings->SetShadowQuality(Preset->ShadowQuality);
	Settings->SetTextureQuality(Preset->TextureQuality);
	Settings->SetPostProcessingQuality(Preset->PostProcessingQuality);
	Settings->SetVisualEffectQuality(Preset->EffectsQuality);
	Settings->SetViewDistanceQuality(Preset->ViewDistanceQuality);
	Settings->SetFoliageQuality(Preset->FoliageQuality);
	Settings->SetOverallScalabilityLevel(Preset->OverallScalability);
	Settings->SetShadingQuality(Preset->ShadingQuality);

	Settings->ApplySettings(false);
}
