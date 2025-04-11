#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Configurations.generated.h"


USTRUCT(BlueprintType)
struct FGraphicsPreset : public FTableRowBase
{
    GENERATED_BODY()

    FGraphicsPreset();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AntiAliasingQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ShadowQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TextureQuality;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShadingQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PostProcessingQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EffectsQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ViewDistanceQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FoliageQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OverallScalability;
};