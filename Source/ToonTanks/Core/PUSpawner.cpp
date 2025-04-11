#include "PUSpawner.h"

#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Skill/PowerUp.h"
#include "../TTGameplayTags.h"

// Sets default values
APUSpawner::APUSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IntervalToTrySpawn = 10.f;
	DropsNumber = 0;
}

// Called when the game starts or when spawned
void APUSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		TimerHandle,
		this,
		&APUSpawner::SpawnPowerUp,
		IntervalToTrySpawn,
		true
	);
}

void APUSpawner::SpawnPowerUp()
{
	UE_LOG(LogTemp, Display, TEXT("Trying to spawn item"));
	if (UKismetMathLibrary::RandomBoolWithWeight(0.6f))
	{
		
		UE_LOG(LogTemp, Display, TEXT("Spawning item somewhere"));
		FVector OriginLocation = GetActorLocation();
		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		FNavLocation ResultLocation;
		NavSystem->GetRandomReachablePointInRadius(OriginLocation, 5000.f, ResultLocation);

		FVector Location = FVector(ResultLocation.Location.X, ResultLocation.Location.Y, 85.f);
		FRotator Rotation = FRotator::ZeroRotator;

		FPassive Passive;
		if (DropsNumber == 7)
		{
			UE_LOG(LogTemp, Display, TEXT("Spawning Health"));
			Passive.MaxDuration = HealthDuration;
			Passive.Modifier = HealthModifier;
			Passive.PassiveType = TTGameplayTags::Attribute_Health;
			DropsNumber = 0;
		}
		else
		{
			RandomizePowerUp(Passive);
			DropsNumber += 1;
		}
		
		APowerUp* PU = GetWorld()->SpawnActor<APowerUp>(PowerUpClass, Location, Rotation);
		PU->SetPassive(Passive);
		PU->SetMesh();
	}
}

void APUSpawner::RandomizePowerUp(FPassive& Passive)
{
	double random = FMath::RandRange(0, (int32)EPassiveType::END - 1);
	

	int32 randomInt = (int32)random;
	GeneratedPassiveType = (EPassiveType)random;

	switch (GeneratedPassiveType)
	{
	case EPassiveType::HEALTH:
	case EPassiveType::FIRE_RATE:
		UE_LOG(LogTemp, Display, TEXT("Fire Rate"));
		Passive.MaxDuration = FireRateDuration;
		Passive.Modifier = FireRateModifier;
		Passive.PassiveType = TTGameplayTags::Attribute_FireRate;
		break;

	case EPassiveType::MOVEMENT_SPEED:
		UE_LOG(LogTemp, Display, TEXT("Movement Speed"));
		Passive.MaxDuration = MovementSpeedDuration;
		Passive.Modifier = MovementSpeedModifier;
		Passive.PassiveType = TTGameplayTags::Attribute_MovementSpeed;
		break;

	case EPassiveType::DAMAGE_LIGHTNING:
		UE_LOG(LogTemp, Display, TEXT("Lightning"));
		Passive.MaxDuration = DamageLightningDuration;
		Passive.Modifier = DamageLightningModifier;
		Passive.PassiveType = TTGameplayTags::Damage_Elemental_Lightning;
		break;

	case EPassiveType::DAMAGE_ICE:
		UE_LOG(LogTemp, Display, TEXT("Ice"));
		Passive.MaxDuration = DamageIceDuration;
		Passive.Modifier = DamageIceModifier;
		Passive.PassiveType = TTGameplayTags::Damage_Elemental_Ice;
		break;

	case EPassiveType::DAMAGE_PHYSICAL:
		UE_LOG(LogTemp, Display, TEXT("Damage Boost"));
		Passive.MaxDuration = DamagePhysicalDuration;
		Passive.Modifier = DamagePhysicalModifier;
		Passive.PassiveType = TTGameplayTags::Damage_Physical;
		break;

	case EPassiveType::SKILL_ACID:
		UE_LOG(LogTemp, Display, TEXT("Acid skill"));
		Passive.PassiveType = TTGameplayTags::Skill_Acid;
		Passive.MaxDuration = SkillAcidDuration;
		break;

	case EPassiveType::SKILL_LASER:
		UE_LOG(LogTemp, Display, TEXT("Laser skill"));
		Passive.PassiveType = TTGameplayTags::Skill_Laser;
		Passive.MaxDuration = SkillLaserDuration;
		break;
	}
}

// Called every frame
void APUSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

