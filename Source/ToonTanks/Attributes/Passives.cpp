#include "Passives.h"

#include "../TTGameplayTags.h"
#include "AttributesComponent.h"
#include "../Skill/Skill.h"
#include "../Character/Tank.h"

void FPassive::Apply(UAttributesComponent* Ref, UWorld* WorldRef)
{
	Duration = MaxDuration;

	// decide which type of passive is
	if (PassiveType.MatchesTag(TTGameplayTags::Damage))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Damage_Physical))
		{
			Ref->DamageStack.Add(this);
			Ref->UpdateDamageModifier();
		}
		else
		{
			if (Ref->ElementalPassive)
			{
				Ref->RemovePassive(Ref->ElementalPassive->PassiveId);
			}

			Ref->ElementalPassive = this;
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Status))
	{
		// reset duration of passives and reset timer to remove
		if (Ref->StatusPassive)
		{
			WorldRef->GetTimerManager().ClearTimer(Ref->StatusPassive->TimerHandle);
			Ref->StatusPassive->Duration = Duration;
			
			FTimerDelegate Del;

			Del.BindLambda([=, *this] {
				Ref->RemovePassive(PassiveId);
			});

			if (WorldRef)
			{
				WorldRef->GetTimerManager().SetTimer(
					Ref->StatusPassive->TimerHandle,
					Del,
					Duration,
					false
				);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Could not create timer, WorldRef is nullptr"));
			}

			if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Bugged))
			{
				bool bFound1 = false;
				bool bFound2 = false;
				for (FPassive& Passive : Ref->Passives)
				{
					if (Passive.PassiveId == Ref->StatusPassive->LinkedPassiveId[0])
					{
						FTimerDelegate DelLinked;

						DelLinked.BindLambda([=, *this] {
							Ref->RemovePassive(PassiveId);
						});

						WorldRef->GetTimerManager().ClearTimer(Passive.TimerHandle);
						Passive.Duration = Duration;

						WorldRef->GetTimerManager().SetTimer(
							Passive.TimerHandle,
							DelLinked,
							Duration,
							false
						);

						bFound1 = true;
					}
					else if (Passive.PassiveId == Ref->StatusPassive->LinkedPassiveId[1])
					{
						FTimerDelegate DelLinked;

						DelLinked.BindLambda([=, *this] {
							Ref->RemovePassive(PassiveId);
							});

						WorldRef->GetTimerManager().ClearTimer(Passive.TimerHandle);
						Passive.Duration = Duration;

						WorldRef->GetTimerManager().SetTimer(
							Passive.TimerHandle,
							DelLinked,
							Duration,
							false
						);
						bFound2 = true;
					}

					if (bFound1 && bFound2)
					{
						UE_LOG(LogTemp, Display, TEXT("Found both Linked Passives, reseted their durations"));
						break;
					}
				}
			}
			else if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Chilled))
			{
				bool bFound1 = false;
				bool bFound2 = false;
				for (FPassive& Passive : Ref->Passives)
				{
					if (Passive.PassiveId == Ref->StatusPassive->LinkedPassiveId[0])
					{
						FTimerDelegate DelLinked;

						DelLinked.BindLambda([=, *this] {
							Ref->RemovePassive(PassiveId);
							});

						WorldRef->GetTimerManager().ClearTimer(Passive.TimerHandle);
						Passive.Duration = Duration;

						WorldRef->GetTimerManager().SetTimer(
							Passive.TimerHandle,
							DelLinked,
							Duration,
							false
						);

						bFound1 = true;
					}
					else if (Passive.PassiveId == Ref->StatusPassive->LinkedPassiveId[1])
					{
						FTimerDelegate DelLinked;

						DelLinked.BindLambda([=, *this] {
							Ref->RemovePassive(PassiveId);
							});

						if (WorldRef)
						{
							WorldRef->GetTimerManager().ClearTimer(Passive.TimerHandle);
							Passive.Duration = Duration;

							WorldRef->GetTimerManager().SetTimer(
								Passive.TimerHandle,
								DelLinked,
								Duration,
								false
							);
							bFound2 = true;
						}
					}

					if (bFound1 && bFound2)
					{
						UE_LOG(LogTemp, Display, TEXT("Found both Linked Passives, reseted their durations"));
						break;
					}
				}
			}

			return;
		}

		ABasePawn* MyOwner = Cast<ABasePawn>(Ref->GetOwner());

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

			LinkedPassiveId.Push(Ref->AddPassive(FireRateDebuff));
			LinkedPassiveId.Push(Ref->AddPassive(MoveSpeedDebuff));

			Ref->StatusPassive = this;
			MyOwner->SetBuggedVFX();
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

			LinkedPassiveId.Push(Ref->AddPassive(FireRateDebuff));
			LinkedPassiveId.Push(Ref->AddPassive(MoveSpeedDebuff));

			Ref->StatusPassive = this;
			MyOwner->SetChilledVFX();
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
				MyOwner->SetSkill(TTGameplayTags::Skill_Acid, TTGameplayTags::SkillNature_Spawner, Duration, PassiveId);
			}
			else if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Laser))
			{
				MyOwner->SetSkill(TTGameplayTags::Skill_Laser, TTGameplayTags::SkillNature_Beam, Duration, PassiveId);
			}
		}
	}
	else if (PassiveType.MatchesTag(TTGameplayTags::Attribute))
	{
		if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_Health))
		{
			Ref->HealthStack.Add(this);
			Ref->UpdateHealthModifier();
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_FireRate))
		{
			Ref->FireRateStack.Add(this);
			Ref->UpdateFireRateModifier();
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Attribute_MovementSpeed))
		{
			Ref->MovementSpeedStack.Add(this);
			Ref->UpdateMovementSpeedModifier();
		}
	}

	if (Duration > 0 && !PassiveType.MatchesTag(TTGameplayTags::Skill))
	{
		FTimerDelegate Del;

		Del.BindLambda([=, *this] {
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
	UE_LOG(LogTemp, Warning, TEXT("Removing passive: %s"), *PassiveType.ToString());
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
		ABasePawn* MyOwner = Cast<ABasePawn>(Ref->GetOwner());
		if (MyOwner == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Owner is nullptr!!!!"));
			return;
		}
		if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Bugged))
		{
			Ref->StatusPassive = nullptr;
			
		}
		else if (PassiveType.MatchesTagExact(TTGameplayTags::Status_Chilled))
		{
			Ref->StatusPassive = nullptr;
		}
		Ref->OnStatusRemoved.Broadcast();
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
				SkillData.SkillType = FGameplayTag::EmptyTag;
				SkillData.SkillNature = FGameplayTag::EmptyTag;
				MyOwner->SetSkillData(SkillData);

			}
			else if (PassiveType.MatchesTagExact(TTGameplayTags::Skill_Laser))
			{
				UE_LOG(LogTemp, Display, TEXT("Removing Laser Skill"));

				FSkillData SkillData;
				SkillData.SkillType = FGameplayTag::EmptyTag;
				SkillData.SkillNature = FGameplayTag::EmptyTag;
				MyOwner->SetSkillData(SkillData);
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

int32 FPassiveStack::GetTotalPassives()
{
	return Passives.Num();
}
