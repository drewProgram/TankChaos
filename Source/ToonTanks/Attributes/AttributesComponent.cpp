#include "AttributesComponent.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/Tank.h"
#include "../Skill/Skill.h"
#include "../TTGameplayTags.h"
#include "../Core/ToonTanksGameMode.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent()
	: BaseHealth(200),
	DamageModifier(0),
	Health(0),
	FireRate(2.f)
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

	// Can change the modifiers
	InitPassives();

	Health = BaseHealth + (BaseHealth * HealthModifier);
	MovementSpeed += MovementSpeed * MovementSpeedModifier;
	FireRate += FireRate * FireRateModifier;
}


void UAttributesComponent::InitPassives()
{
	// Pass through all passives and apply the modifiers
	//if (Passives.IsEmpty())
	//{
	//	UE_LOG(LogTemp, Display, TEXT("Passives array of %s is empty"), *GetOwner()->GetActorLabel());
	//	return;
	//}

	//// check if has global modifiers to apply
	//for (FPassive Passive : Passives)
	//{
	//	Passive.Apply(this);
	//}
}

void UAttributesComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f) return;
	if (DamagedActor)
	{
		Health -= Damage;

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

int32 UAttributesComponent::GetHealth()
{
	return Health;
}

void UAttributesComponent::UpdateHealthModifier()
{
	HealthModifier = HealthStack.GetTotalModifier() / 100;
	Health = BaseHealth + (BaseHealth * HealthModifier);
}

float UAttributesComponent::GetFireRate()
{
	return FireRate;
}

void UAttributesComponent::UpdateFireRateModifier()
{
	FireRateModifier = FireRateStack.GetTotalModifier() / 100;
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

void UAttributesComponent::UpdateMovementSpeedModifier()
{
	MovementSpeedModifier = MovementSpeedStack.GetTotalModifier() / 100;
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
