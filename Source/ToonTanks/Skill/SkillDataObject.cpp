// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataObject.h"

USkillDataObject::USkillDataObject()
{
}

FOnSkillStartedDelegate& USkillDataObject::GetOnSkillStarted()
{
	return SkillData.OnSkillStarted;
}

FOnSkillEndedDelegate& USkillDataObject::GetOnSkillEnded()
{
	return SkillData.OnSkillEnded;
}

const FSkillData& USkillDataObject::GetSkillData() const
{
	return SkillData;
}
