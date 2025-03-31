// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"

#include "GameplayTagContainer.h"

#include "../Skill/Skill.h"
#include "../Skill/SkillDataObject.h"

#include "Tank.generated.h"

class ULegacyCameraShake;

// ClientStartCameraShake

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorGotSkillDelegate);

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

	FOnActorGotSkillDelegate OnActorGotSkill;

	virtual void HandleDestruction() override;

	UFUNCTION(BlueprintImplementableEvent)
	void SetBPSkill(FGameplayTag SkillType);

	FVector GetTurretLookDirection();

	UFUNCTION(BlueprintPure)
	USkillDataObject* GetSkillDataObject() const;

	UFUNCTION(BlueprintCallable)
	void SetSkillClass(TSubclassOf<class ASkill> SkillClass, TSubclassOf<class ASkillSpawner> SkillSpawner);

	void SetSkill(FGameplayTag SkillType, FGameplayTag SkillNature, float Duration, FGuid Id);
	void SetSkillData(FSkillData Data);

	void RequestSkillCast();

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer TagContainer;

protected:
	virtual void BeginPlay() override;

	void ShootSpecial();

	UPROPERTY(EditAnywhere)
	float TurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, BlueprintGetter = GetSkillDataObject, meta = (AllowPrivateAccess = "true"))
	USkillDataObject* SkillDataObj;
};
