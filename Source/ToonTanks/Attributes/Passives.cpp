#include "Passives.h"

#include "../TTGameplayTags.h"
#include "AttributesComponent.h"
#include "../Skill/Skill.h"
#include "../Character/Tank.h"

void FPassive::Apply(UAttributesComponent* Ref, UWorld* WorldRef)
{
	bool bPassiveExists = Ref->DoesPassiveAlreadyExists(*this);

	Duration = MaxDuration;

	// decide which type of passive is
	if (PassiveType.MatchesTag(TTGameplayTags::Damage))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Physical))
		{
			Ref->DamageStack.Add(this);
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Elemental_Ice))
		{
			Ref->ElementalPassive = this;
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Elemental_Lightning))
		{
			Ref->ElementalPassive = this;
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Status))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Bugged))
		{
			FPassive MoveSpeedDebuff;
			FPassive FireRateDebuff;

			MoveSpeedDebuff.PassiveType = TTGameplayTags::Attribute_MovementSpeed;
			MoveSpeedDebuff.Modifier = Modifier;
			MoveSpeedDebuff.MaxDuration = Duration;

			FireRateDebuff.PassiveType = TTGameplayTags::Attribute_FireRate;
			FireRateDebuff.Modifier = Modifier;
			FireRateDebuff.MaxDuration = Duration;

			Ref->AddPassive(FireRateDebuff);
			Ref->AddPassive(MoveSpeedDebuff);

			Ref->StatusPassive = this;
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Chilled))
		{
			FPassive MoveSpeedDebuff;
			FPassive FireRateDebuff;

			MoveSpeedDebuff.PassiveType = TTGameplayTags::Attribute_MovementSpeed;
			MoveSpeedDebuff.Modifier = Modifier;
			MoveSpeedDebuff.MaxDuration = Duration;

			FireRateDebuff.PassiveType = TTGameplayTags::Attribute_FireRate;
			FireRateDebuff.Modifier = Modifier;
			FireRateDebuff.MaxDuration = Duration;

			Ref->AddPassive(FireRateDebuff);
			Ref->AddPassive(MoveSpeedDebuff);

			Ref->StatusPassive = this;
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Skill))
	{
		ATank* MyOwner = Cast<ATank>(Ref->GetOwner());
		if (MyOwner == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Owner is nullptr!!!!"));
		}
		else
		{
			if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Acid))
			{
				UE_LOG(LogTemp, Display, TEXT("Applying acid skill"));
			}
			else if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Laser))
			{
				FSkillData SkillData;
				MyOwner->GetSkillData(SkillData);

				SkillData.SkillType = TTGameplayTags::Skill_Laser;
			}
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Attribute))
	{
		UE_LOG(LogTemp, Display, TEXT("Entrou em Attribute"));
		if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_Health))
		{
			UE_LOG(LogTemp, Display, TEXT("Entrou em Attribute Health"));
			Ref->HealthStack.Add(this);
			Ref->UpdateHealthModifier();
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_FireRate))
		{
			Ref->FireRateStack.Add(this);
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_MovementSpeed))
		{
			Ref->MovementSpeedStack.Add(this);
		}
	}

	if (Duration > 0)
	{
		FTimerDelegate Del;

		UE_LOG(LogTemp, Warning, TEXT("Antes da lambda; PassiveId fora: %s"), *PassiveId.ToString());

		Del.BindLambda([=, *this] {
			UE_LOG(LogTemp, Warning, TEXT("Passando por lambda; PassiveId dentro: %s"), *PassiveId.ToString());
			Ref->RemovePassive(PassiveId);
		});

		if (WorldRef)
		{
			WorldRef->GetTimerManager().SetTimer(
				TimerHandle,
				Del,
				Duration,
				false
			);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not create timer, WorldRef is nullptr"));
		}
	}
}

void FPassive::Remove(UAttributesComponent* Ref)
{
	UE_LOG(LogTemp, Warning, TEXT("Passive type: %s"), *PassiveType.ToString());
	// decide which type of passive is
	if (PassiveType.MatchesTag(TTGameplayTags::Damage))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Physical))
		{
			Ref->DamageStack.Remove(this);
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Elemental_Ice))
		{
			Ref->ElementalPassive = nullptr;
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Elemental_Lightning))
		{
			Ref->ElementalPassive = nullptr;
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Status))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Bugged))
		{
			Ref->StatusPassive = nullptr;
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Chilled))
		{
			Ref->StatusPassive = nullptr;
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Skill))
	{
		ATank* MyOwner = Cast<ATank>(Ref->GetOwner());
		if (MyOwner == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Owner is nullptr!!!!"));
		}
		else
		{
			if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Acid))
			{
				UE_LOG(LogTemp, Display, TEXT("Removing acid skill"));

				FSkillData SkillData;
				MyOwner->GetSkillData(SkillData);

				SkillData.SkillType = FGameplayTag::EmptyTag;
			}
			else if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Laser))
			{
				FSkillData SkillData;
				MyOwner->GetSkillData(SkillData);

				SkillData.SkillType = FGameplayTag::EmptyTag;

				UE_LOG(LogTemp, Display, TEXT("Removing Laser Skill"));
			}
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Attribute))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_Health))
		{
			Ref->HealthStack.Remove(this);
			Ref->UpdateHealthModifier();
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_FireRate))
		{
			Ref->FireRateStack.Remove(this);
			Ref->UpdateFireRateModifier();
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_MovementSpeed))
		{
			Ref->MovementSpeedStack.Remove(this);
			Ref->UpdateMovementSpeedModifier();
		}
	}
}

bool FPassive::operator==(const FPassive& Other)
{
	return PassiveId == Other.PassiveId;
}

void FPassiveStack::Add(FPassive* Passive)
{
	UE_LOG(LogTemp, Display, TEXT("Adding Passive %s"), *Passive->PassiveType.ToString());
	TotalModifier += Passive->Modifier;
	Passives.Push(Passive);
}

void FPassiveStack::Remove(FPassive* Passive)
{
	TotalModifier -= Passive->Modifier;
	Passives.Remove(Passive);
}

float FPassiveStack::GetTotalModifier()
{
	return TotalModifier;
}