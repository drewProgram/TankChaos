#include "Tank.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraShakeBase.h"

#include "../TTGameplayTags.h"
#include "../Attributes/AttributesComponent.h"
#include "../Core/SkillSpawner.h"

ATank::ATank()
	: TurnRate(200.f)
{
	SkillData.Owner = this;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TagContainer.HasTagExact(TTGameplayTags::State_EndingCastSkill))
	{
		TagContainer.RemoveTag(TTGameplayTags::State_EndingCastSkill);
		UE_LOG(LogTemp, Display, TEXT("Skill ended"));
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ATank::SetSkillClass(TSubclassOf<class ASkill> SkillClass, TSubclassOf<class ASkillSpawner> SkillSpawner)
{
	SkillData.SkillClass = SkillClass;
	SkillData.SkillSpawner = SkillSpawner;
}

void ATank::SetSkill(FGameplayTag SkillType, FGameplayTag SkillNature, float Duration, FGuid Id)
{
	// override skill if it's different from current skill
	if (SkillData.SkillType.IsValid() && !SkillData.SkillType.MatchesTagExact(SkillType))
	{
		AttributesComponent->RemovePassive(SkillData.PassiveId);
	}

	SkillData.SkillType = SkillType;
	SkillData.SkillNature = SkillNature;
	SkillData.PassiveId = Id;
	SkillData.Duration = Duration;
	if (SkillType.MatchesTagExact(TTGameplayTags::Skill_Laser))
	{
		SkillData.Damage = 5.f;
		SkillData.Range = 1000.0f;
		SkillData.MaxUses = 5;
	}
	else if (SkillType.MatchesTagExact(TTGameplayTags::Skill_Acid))
	{
		SkillData.bSpawnsFromProjectile = true;
		SkillData.Damage = 10.f;
		SkillData.Range = 200.0f;
		SkillData.MaxUses = 5;
	}
	SkillData.UpdateSkillCount();
	SetBPSkill(SkillType);
}

void ATank::SetSkillData(FSkillData Data)
{
	SkillData = Data;
}

FVector ATank::GetTurretLookDirection()
{
	return TurretMesh->GetForwardVector();
}

void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::ShootSpecial()
{
	if (SkillData.SkillClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Shooting special"));
		if (SkillData.SkillType.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("Skill Type valid"));

			if (SkillData.GetUsesLeft() <= 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("0 uses left of skill"));
				return;
			}

			FGameplayTagContainer StatesToCheck;
			StatesToCheck.AddTag(TTGameplayTags::State_CastingSkill);
			StatesToCheck.AddTag(TTGameplayTags::State_StartingCastSkill);
			StatesToCheck.AddTag(TTGameplayTags::State_EndingCastSkill);

			if (TagContainer.HasAnyExact(StatesToCheck))
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't use special now"));
				return;
			}


			TagContainer.AddTag(TTGameplayTags::State_StartingCastSkill);

			SkillData.Owner = this;
			if (SkillData.RequestCastSkill(ProjectileSpawnPoint->GetComponentLocation(), GetWorld(), TurretMesh->GetComponentRotation()))
			{
				TagContainer.RemoveTag(TTGameplayTags::State_StartingCastSkill);
				TagContainer.AddTag(TTGameplayTags::State_CastingSkill);
			}
			else
			{
				TagContainer.RemoveTag(TTGameplayTags::State_StartingCastSkill);
			}
		}
	}
}
