#include "BasePawn.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "../Weapons/Projectile.h"
#include "../Attributes/HealthComponent.h"
#include "../Attributes/AttributesComponent.h"
#include "../TTGameplayTags.h"


// Sets default values
ABasePawn::ABasePawn()
	: MoveSpeed(400.f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>(TEXT("Attributes Component"));
}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABasePawn::Shoot()
{
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	if (Projectile)
	{
		Projectile->SetOwner(this);
		AActor* PO = Projectile->GetOwner();
		if (PO != nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("Owner set successfuly. Owner: %s"), *PO->GetActorLabel());
			
			Projectile->SetStats(AttributesComponent->GetDamageModifier());
			FGameplayTagContainer ProjectileTags;
			if (!AttributesComponent->GetElementalDamage().PassiveId.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Don't have elemental ammo. Normal damage"));
				ProjectileTags.AddTag(TTGameplayTags::Damage_Physical);
				Projectile->ApplyCustomTags(ProjectileTags);
			}
			else
			{
				UE_LOG(LogTemp, Display, TEXT("Passive: %s"), *AttributesComponent->GetElementalDamage().PassiveType.ToString());
				ProjectileTags.AddTag(AttributesComponent->GetElementalDamage().PassiveType);
				Projectile->ApplyCustomTags(ProjectileTags);
			}
		}
	}
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABasePawn::HandleDestruction()
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
}

UAttributesComponent* ABasePawn::GetAttributesComponent() const
{
	return AttributesComponent;
}

USceneComponent* ABasePawn::GetProjectileSpawnPoint()
{
	return ProjectileSpawnPoint;
}

UStaticMeshComponent* ABasePawn::GetTurretMeshComponent()
{
	return TurretMesh;
}

void ABasePawn::SetTurretRotation(FRotator Rotator)
{
	TurretMesh->SetWorldRotation(Rotator);
}
