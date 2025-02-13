// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Skill.h"

#include "SkillDataObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOONTANKS_API USkillDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	USkillDataObject();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skills")
	FSkillData SkillData;

private:
	FSkillData TmpSkillData;
};
