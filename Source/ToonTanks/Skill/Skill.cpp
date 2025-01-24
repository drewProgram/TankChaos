#include "Skill.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/ShapeComponent.h"

#include "../Character/Tank.h"
#include "../Character/Tower.h"
#include "../Core/DamageHandlerComponent.h"
#include "../TTGameplayTags.h"
#include "../SkillSpawner.h"
#include "../Attributes/AttributesComponent.h"

// Collision channel for skills: ECC_GameTraceChannel1

FSkillData::FSkillData()
{
	HasSkillEnded = true;
	SkillClass = nullptr;

	Owner = nullptr;
	SkillType = FGameplayTag::EmptyTag;
	Duration = 1.f;
	bSpawnsFromProjectile = false;
}

int32 FSkillData::GetUsesLeft()
{
	return UsesLeft;
}

bool FSkillData::RequestCastSkill(FVector SpawnLocation, UWorld* WorldRef, FRotator Rotation)
{
	if (!HasSkillEnded && SkillNature.MatchesTag(TTGameplayTags::SkillNature_Spawner))
	{
		return false;
	}

	if (!WorldRef) return false;

	if (UsesLeft <= 0) return false;


	UE_LOG(LogTemp, Display, TEXT("Requesting skill"));
	UE_LOG(LogTemp, Warning, TEXT("Skill nature: %s"), *SkillNature.GetTagName().ToString());
	HasSkillEnded = false;

	FVector LineStart = SpawnLocation;
	FVector LineEnd = LineStart + (Owner->GetTurretLookDirection() * Range);
	FTimerHandle TimerHandle;
	FTimerDelegate Del;

	if (SkillClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Skill class exists"));
		if (SkillNature.MatchesTagExact(TTGameplayTags::SkillNature_Spawner))
		{
			ASkillSpawner* SkillSpawnerRef = WorldRef->SpawnActor<ASkillSpawner>(SkillSpawner, SpawnLocation, Rotation);
			SkillSpawnerRef->SetOwner(Owner);
			SkillSpawnerRef->SetSkillData(*this);

			Del.BindLambda([this] {
				NotifySkillEnded();
				});

			// timer to prevent many skill projectiles being shot at the same spot
			WorldRef->GetTimerManager().SetTimer(
				TimerHandle,
				Del,
				0.5f,
				false
			);

			UsesLeft -= 1;

			return true;
		}
		else
		{
			ASkill* SkillRef = WorldRef->SpawnActor<ASkill>(SkillClass, SpawnLocation, Rotation);
			SkillRef->SetupSkillData(*this);
			UE_LOG(LogTemp, Display, TEXT("Skill class name: %s"), *SkillRef->GetActorNameOrLabel());
		}

		Del.BindLambda([this] {
			NotifySkillEnded();
			});

		// Outside classes we need to use a timer delegate to call a function (use a lambda to call the function you want)
		WorldRef->GetTimerManager().SetTimer(
			TimerHandle,
			Del,
			Duration,
			false
		);

		UsesLeft -= 1;

		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Skill class is nullptr"));

	return false;
}

void FSkillData::NotifySkillEnded()
{
	UE_LOG(LogTemp, Display, TEXT("Tags start skill: %s"), *Owner->TagContainer.ToString());
	Owner->TagContainer.RemoveTag(TTGameplayTags::State_CastingSkill);
	Owner->TagContainer.AddTag(TTGameplayTags::State_EndingCastSkill);
	UE_LOG(LogTemp, Display, TEXT("Tags end skill: %s"), *Owner->TagContainer.ToString());
	if (UsesLeft == 0)
	{
		Owner->GetAttributesComponent()->RemovePassive(PassiveId);
	}

	HasSkillEnded = true;
}

void FSkillData::UpdateSkillCount()
{
	UsesLeft = MaxUses;
}

// Sets default values
ASkill::ASkill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkillParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Skill Particle"));
	RootComponent = SkillParticle;

	DamageHandlerComponent = CreateDefaultSubobject<UDamageHandlerComponent>(TEXT("DamageHandlerComponent"));

	bTimerSet = false;
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Skill spawned!"));
}

void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO: instead of updating each frame the position, attach this actor to the SpawnPoint in blueprint.
	if (SkillData.SkillType.IsValid())
	{
		if (SkillData.SkillType.MatchesTagExact(TTGameplayTags::Skill_Laser))
		{
			FVector NewLocation = SkillData.Owner->GetProjectileSpawnPoint()->GetComponentLocation();
			FQuat NewQuat = SkillData.Owner->GetTurretMeshComponent()->GetComponentRotation().Quaternion();

			SetActorLocationAndRotation(NewLocation, NewQuat);
		}
		TArray<AActor*> OverlappingActors;
		SkillCollider->GetOverlappingActors(OverlappingActors);

		for (AActor* OverlappedActor : OverlappingActors)
		{
			if (OverlappedActor != SkillData.Owner)
			{
				UE_LOG(LogTemp, Warning, TEXT("OverlappedActor: %s"), *OverlappedActor->GetName());
				if (!DamageHandlerComponent->TryDamage(OverlappedActor, SkillData.Damage))
				{
					UE_LOG(LogTemp, Display, TEXT("Can't damage actor now!"));
				}
			}
		}
	}
}

void ASkill::SetDestroyTimer()
{
	FTimerHandle TimerHandle;
	UE_LOG(LogTemp, Display, TEXT("Timer set to %f seconds"), SkillData.Duration);
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&ASkill::DestroyTest,
		SkillData.Duration,
		false
	);
}

void ASkill::DestroyTest()
{
	UE_LOG(LogTemp, Display, TEXT("Destroying skill!"));
	Destroy();
}
