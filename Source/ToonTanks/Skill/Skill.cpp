#include "Skill.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

#include "../Character/Tank.h"
#include "../TTGameplayTags.h"
#include "../Attributes/AttributesComponent.h"

FSkillData::FSkillData()
{
	HasSkillEnded = true;
	SkillClass = nullptr;

	Owner = nullptr;
	SkillType = FGameplayTag::EmptyTag;
}

int32 FSkillData::GetUsesLeft()
{
	return UsesLeft;
}

bool FSkillData::RequestCastSkill(FVector SpawnLocation, float Range, UWorld* WorldRef)
{
	if (!HasSkillEnded)
	{
		return false;
	}

	if (!WorldRef) return false;

	if (UsesLeft <= 0) return false;

	UE_LOG(LogTemp, Display, TEXT("Requesting skill"));
	HasSkillEnded = false;

	if (SkillType.MatchesTagExact(TTGameplayTags::Skill_Laser))
	{

	}

	FVector LineStart = SpawnLocation;
	FVector LineEnd = LineStart + (Owner->GetTurretLookDirection() * Range);
	DrawDebugLine(WorldRef, LineStart, LineEnd, FColor::Red, false, 2.f, 0, 4.f);
	FTimerHandle TimerHandle;
	FTimerDelegate Del;

	if (SkillClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Skill class exists"));
		ASkill* SkillRef = WorldRef->SpawnActor<ASkill>(SkillClass, SpawnLocation, FRotator());


		UE_LOG(LogTemp, Display, TEXT("Skill class name: %s"), *SkillRef->GetActorNameOrLabel());
	}

	Del.BindLambda([this] {
		NotifySkillEnded();
	});

	// Outside classes we need to use a timer delegate to call a function (use a lambda to call the function you want)
	WorldRef->GetTimerManager().SetTimer(
		TimerHandle,
		Del,
		1.f,
		false
	);

	// skill pointer
	// ptr->OnSkillEnded.BindUObject(this, &ref, this)
	
	UsesLeft -= 1;

	return true;
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

	SkillMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Skill Mesh"));
	RootComponent = SkillMesh;

	CapsuleCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	CapsuleCollider->SetupAttachment(SkillMesh);
}

// Called when the game starts or when spawned
void ASkill::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT("Skill spawned!"));

	CapsuleCollider->OnComponentBeginOverlap.AddDynamic(this, &ASkill::BeginOverlap);
}

void ASkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkill::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Display, TEXT("Being overlaped by: %s"), *OtherActor->GetActorNameOrLabel());
}