#include "AttributesComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "../Character/Tank.h"
#include "../Character/Tower.h"
#include "../Skill/Skill.h"
#include "../Weapons/Projectile.h"
#include "../TTGameplayTags.h"
#include "../Core/ToonTanksGameMode.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
	: BaseHealth(200),
	MaxHealth(0),
	DamageModifier(0),
	Health(0),
	FireRate(0),
	BaseFireRate(60.f),
	BaseMovementSpeed(500.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttributesComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAttributesComponent::DamageTaken);

	// Can change the modifiers
	InitPassives();

	MaxHealth = BaseHealth + (BaseHealth * HealthModifier);
	Health = MaxHealth;
	MovementSpeed = BaseMovementSpeed + (BaseMovementSpeed * MovementSpeedModifier);
	FireRate = BaseFireRate + (BaseFireRate * FireRateModifier);
}


void UAttributesComponent::InitPassives()
{
	AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		HealthModifier = GameMode->GetEnemyHealthBonus() / 100;
		DamageModifier = GameMode->GetEnemyDamageBonus() / 100;
	}
}

void UAttributesComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	if (DamagedActor)
	{
		Health -= Damage;

		if (ATower* Tower = Cast<ATower>(DamagedActor))
		{
			if (AProjectile* Projectile = Cast<AProjectile>(DamageCauser))
			{
				FGameplayTag DamageTag = Projectile->GetDamageType();

				// TODO: add easier way to control debuff properties (edit values in the editor, not in code)
				if (DamageTag.MatchesTag(TTGameplayTags::Damage_Elemental_Lightning))
				{
					UE_LOG(LogTemp, Display, TEXT("Lightning Damage!!"));
					if (UKismetMathLibrary::RandomBoolWithWeight(0.5f))
					{
						// 50% chance of stun
						UE_LOG(LogTemp, Warning, TEXT("Stun!!"));
						FPassive Stun;

						Stun.PassiveType = TTGameplayTags::Status_Bugged;
						Stun.Modifier = -100.f;
						Stun.MaxDuration = 15.f;

						ABasePawn* MyOwner = Cast<ABasePawn>(GetOwner());
						OnStatusApplied.Broadcast(Stun.PassiveType, Stun.MaxDuration);
						MyOwner->SetBuggedVFX();
						AddPassive(Stun);
					}
				}
				else if (DamageTag.MatchesTag(TTGameplayTags::Damage_Elemental_Ice))
				{
					UE_LOG(LogTemp, Display, TEXT("Ice Damage!!"));
					// 60% chance of slow
					if (UKismetMathLibrary::RandomBoolWithWeight(0.6f))
					{
						UE_LOG(LogTemp, Warning, TEXT("Slow!!"));

						FPassive Chill;

						Chill.PassiveType = TTGameplayTags::Status_Chilled;
						Chill.Modifier = -50.f;
						Chill.MaxDuration = 2.f;

						ABasePawn* MyOwner = Cast<ABasePawn>(GetOwner());
						MyOwner->SetChilledVFX();
						OnStatusApplied.Broadcast(Chill.PassiveType, Chill.MaxDuration);
						AddPassive(Chill);
					}
				}
			}
		}

		OnHealthUpdated.Broadcast();

		if (Health <= 0)
		{
			AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
			if (GameMode)
			{
				GameMode->ActorDied(DamagedActor);
			}
		}
	}
}

// Called every frame
void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

int32 UAttributesComponent::GetHealth() const
{
	return Health;
}

int32 UAttributesComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UAttributesComponent::UpdateHealthModifier()
{
	HealthModifier = HealthStack.GetTotalModifier() / 100;
	MaxHealth = BaseHealth + (BaseHealth * HealthModifier);
	Health = MaxHealth;
	OnHealthUpdated.Broadcast();
}

float UAttributesComponent::GetFireRate()
{
	if (FireRate <= 0)
	{
		UE_LOG(LogTemp, Display, TEXT("retuning 0"));
		return 0;
	}

	UE_LOG(LogTemp, Display, TEXT("FireRate: %f"), FireRate);
	float FireRateSeconds = 60 / FireRate;
	return FireRateSeconds;
}

void UAttributesComponent::UpdateFireRateModifier()
{
	FireRateModifier = FireRateStack.GetTotalModifier() / 100;
	FireRate = BaseFireRate + (BaseFireRate * FireRateModifier);
}

void UAttributesComponent::UpdateDamageModifier()
{
	DamageModifier = DamageStack.GetTotalModifier() / 100;

}

float UAttributesComponent::GetDamageModifier()
{
	return DamageModifier;
}

float UAttributesComponent::GetMovementSpeed()
{
	return MovementSpeed;
}

FPassive UAttributesComponent::GetElementalDamage()
{
	return ElementalPassive;
}

void UAttributesComponent::UpdateMovementSpeedModifier()
{
	MovementSpeedModifier = MovementSpeedStack.GetTotalModifier() / 100;
	MovementSpeed = BaseMovementSpeed + (BaseMovementSpeed * MovementSpeedModifier);
}

FPassive UAttributesComponent::GetStatusPassive()
{
	return StatusPassive;
}

FPassive UAttributesComponent::GetElementalPassive()
{
	return ElementalPassive;
}

void UAttributesComponent::AddPassive(FPassive Passive)
{
	UE_LOG(LogTemp, Warning, TEXT("Número de passivas: %d"), Passives.Num());

	Passive.PassiveId = FGuid::NewGuid();

	Passives.Add(Passive);

	Passive.Apply(this, GetWorld());

	UE_LOG(LogTemp, Warning, TEXT("Número de passivas: %d"), Passives.Num());

}

void UAttributesComponent::RemovePassive(FGuid Id)
{
	UE_LOG(LogTemp, Warning, TEXT("Passando por remove passive. FGuid: %s"), *Id.ToString());
	TArray<FPassive> PassivesTemp;
	PassivesTemp = Passives;
	bool bFound = false;
	for (FPassive& Passive : Passives)
	{
		if (Passive.PassiveId == Id)
		{
			// c++ doesn't let modify element while is being iterated, so we create a temp array and copy the changes later
			PassivesTemp.Remove(Passive);

			FPassive PassiveTemp = Passive;
			PassiveTemp.Remove(this);

			bFound = true;

			break;
		}
	}

	if (bFound)
	{
		Passives = PassivesTemp;
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not find passive!"));
}

bool UAttributesComponent::DoesPassiveAlreadyExists(FPassive Passive)
{/*
	for (const FPassive& P : Passives)
	{
		if (P.PassiveType.MatchesTagExact(Passive.PassiveType))
		{
			return true;
		}
	}*/
	return false;
}
