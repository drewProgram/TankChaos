#include "SkillSpawner.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../TTGameplayTags.h"

// Sets default values
ASkillSpawner::ASkillSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projetile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;
}

// Called when the game starts or when spawned
void ASkillSpawner::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ASkillSpawner::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}
}

void ASkillSpawner::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());

	if (SkillData.SkillClass)
	{
		SkillData.SkillNature = TTGameplayTags::SkillNature_Ground;

		SkillData.RequestCastSkill(Hit.ImpactPoint, GetWorld(), GetActorRotation());
	}

	Destroy();
}

// Called every frame
void ASkillSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillSpawner::SetSkillData(FSkillData SD)
{
	SkillData = SD;
}

