// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameplayTagContainer.h"

#include "../Skill/Skill.h"

#include "Tank.generated.h"

class ULegacyCameraShake;

// ClientStartCameraShake

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()
	
public:
	ATank();

	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetBPSkill(FGameplayTag SkillType);

	FVector GetTurretLookDirection();

	UFUNCTION(BlueprintCallable)
	void SetSkillClass(TSubclassOf<class ASkill> SkillClass, TSubclassOf<class ASkillSpawner> SkillSpawner);

	void SetSkill(FGameplayTag SkillType, FGameplayTag SkillNature, float Duration, FGuid Id);
	void SetSkillData(FSkillData Data);

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer TagContainer;

protected:
	virtual void BeginPlay() override;

	void ShootSpecial();

	UPROPERTY(EditAnywhere)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FSkillData SkillData;
};
