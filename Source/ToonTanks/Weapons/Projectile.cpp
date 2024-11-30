#include "Projectile.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"

#include "../Character/BasePawn.h"
#include "../TTGameplayTags.h"
#include "../Attributes/AttributesComponent.h"

// Sets default values
AProjectile::AProjectile()
	: BaseDamage(50.f), DamageModifier(0.f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = ProjectileMesh;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projetile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = 1300.f;
	ProjectileMovementComponent->InitialSpeed = 1300.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	ABasePawn* MyOwner = Cast<ABasePawn>(GetOwner());
	if (MyOwner)
	{
		DamageModifier = MyOwner->GetAttributesComponent()->GetDamageModifier();
		Damage = BaseDamage + (BaseDamage * DamageModifier);
	}

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}


	/*if (HitCameraShake)
	{

	}*/
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::ApplyCustomTags(FGameplayTagContainer TagContainer)
{
	if (TagContainer.IsEmpty())
	{
		return;
	}

	if (TagContainer.HasTag(TTGameplayTags::Damage_Elemental))
	{
		if (TagContainer.HasTagExact(TTGameplayTags::Damage_Elemental_Ice))
		{
			DamageType = TTGameplayTags::Damage_Elemental_Ice;
		}
		else
		{
			DamageType = TTGameplayTags::Damage_Elemental_Lightning;
		}
	}
	else
	{
		DamageType = TTGameplayTags::Damage_Physical;
	}
}

void AProjectile::SetStats(float DmgModifier)
{
	DamageModifier = DmgModifier;
	Damage = BaseDamage + (BaseDamage * DamageModifier);
}

FGameplayTag AProjectile::GetDamageType()
{
	return DamageType;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Owner null"));
		Destroy();
		return;
	}


	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();


	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UE_LOG(LogTemp, Display, TEXT("Will apply %f damage"), Damage);
		UE_LOG(LogTemp, Display, TEXT("Damage of type: %s"), *DamageType.ToString());
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}

	Destroy();
}
