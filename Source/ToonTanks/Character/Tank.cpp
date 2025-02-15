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
	SkillDataObj = CreateDefaultSubobject<USkillDataObject>(TEXT("SkillDataObject"));
	SkillDataObj->SkillData.Owner = this;
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

USkillDataObject* ATank::GetSkillDataObject() const
{
	return SkillDataObj;
}

void ATank::SetSkillClass(TSubclassOf<class ASkill> SkillClass, TSubclassOf<class ASkillSpawner> SkillSpawner)
{
	SkillDataObj->SkillData.SkillClass = SkillClass;
	SkillDataObj->SkillData.SkillSpawner = SkillSpawner;
}

void ATank::SetSkill(FGameplayTag SkillType, FGameplayTag SkillNature, float Duration, FGuid Id)
{
	// override skill if it's different from current skill
	if (SkillDataObj->SkillData.SkillType.IsValid() && !SkillDataObj->SkillData.SkillType.MatchesTagExact(SkillType))
	{
		AttributesComponent->RemovePassive(SkillDataObj->SkillData.PassiveId);
	}

	SkillDataObj->SkillData.SkillType = SkillType;
	SkillDataObj->SkillData.SkillNature = SkillNature;
	SkillDataObj->SkillData.PassiveId = Id;
	SkillDataObj->SkillData.Duration = Duration;
	if (SkillType.MatchesTagExact(TTGameplayTags::Skill_Laser))
	{
		SkillDataObj->SkillData.Damage = 5.f;
		SkillDataObj->SkillData.Range = 1000.0f;
		SkillDataObj->SkillData.MaxUses = 5;
	}
	else if (SkillType.MatchesTagExact(TTGameplayTags::Skill_Acid))
	{
		SkillDataObj->SkillData.bSpawnsFromProjectile = true;
		SkillDataObj->SkillData.Damage = 10.f;
		SkillDataObj->SkillData.Range = 200.0f;
		SkillDataObj->SkillData.MaxUses = 5;
	}
	SkillDataObj->SkillData.UpdateSkillCount();
	SetBPSkill(SkillType);

	OnActorGotSkill.Broadcast();
}

void ATank::SetSkillData(FSkillData Data)
{
	SkillDataObj->SkillData = Data;
}

void ATank::RequestSkillCast()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to cast skill"));
	ShootSpecial();
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
	if (SkillDataObj->SkillData.SkillClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Shooting special"));
		if (SkillDataObj->SkillData.SkillType.IsValid())
		{
			UE_LOG(LogTemp, Display, TEXT("Skill Type valid"));

			if (SkillDataObj->SkillData.GetUsesLeft() <= 0)
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

			SkillDataObj->SkillData.Owner = this;
			if (SkillDataObj->SkillData.RequestCastSkill(ProjectileSpawnPoint->GetComponentLocation(), GetWorld(), TurretMesh->GetComponentRotation()))
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
