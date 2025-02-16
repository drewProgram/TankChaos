// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Skill.h"

#include "SkillDataObject.generated.h"

UCLASS(BlueprintType)
class TOONTANKS_API USkillDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	USkillDataObject();

	UFUNCTION(BlueprintPure)
	const FSkillData& GetSkillData() const;

	UFUNCTION()
	FOnSkillStartedDelegate& GetOnSkillStarted();

	UFUNCTION()
	FOnSkillEndedDelegate& GetOnSkillEnded();

	UFUNCTION()
	FOnSkillSlotRemovedDelegate& GetOnSkillSlotRemoved();

private:
	friend class ATank;
	friend class ATankNPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, BlueprintGetter = GetSkillData, Category = "Skills", meta = (AllowPrivateAccess="true"))
	FSkillData SkillData;
};
