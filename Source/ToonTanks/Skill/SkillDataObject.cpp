// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillDataObject.h"

USkillDataObject::USkillDataObject()
{
	UE_LOG(LogTemp, Display, TEXT("Skill Object created"));
}

FOnSkillStartedDelegate& USkillDataObject::GetOnSkillStarted()
{
	return SkillData.OnSkillStarted;
}

FOnSkillEndedDelegate& USkillDataObject::GetOnSkillEnded()
{
	return SkillData.OnSkillEnded;
}

FOnSkillSlotRemovedDelegate& USkillDataObject::GetOnSkillSlotRemoved()
{
	return SkillData.OnSkillSlotRemoved;
}

void USkillDataObject::RequestSkillCancel()
{
	UE_LOG(LogTemp, Display, TEXT("Trying to cancel skill"));
	SkillData.UsesLeft = 0;
	SkillData.CancelCurrentSkill();
}

const FSkillData& USkillDataObject::GetSkillData() const
{
	return SkillData;
}
