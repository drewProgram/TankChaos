#include "DamageHandlerComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDamageHandlerComponent::UDamageHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UDamageHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UDamageHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UDamageHandlerComponent::TryDamage(AActor* ActorToDamage, float Damage)
{
	if (CooldownActors.Find(ActorToDamage) > 0) return false;

	CooldownActors.Add(ActorToDamage);

	FTimerHandle TimerHandle;
	FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &UDamageHandlerComponent::RemoveCooldownActor, ActorToDamage);

	AController* MyOwnerInstigator = GetOwner()->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	UE_LOG(LogTemp, Display, TEXT("Damaging actor with skill!"));

	UGameplayStatics::ApplyDamage(ActorToDamage, Damage, MyOwnerInstigator, GetOwner(), DamageTypeClass);

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		Del,
		0.5f,
		false
	);

	return true;
}

void UDamageHandlerComponent::RemoveCooldownActor(AActor* Actor)
{
	UE_LOG(LogTemp, Display, TEXT("Removing cooldown from actor!"));
	CooldownActors.Remove(Actor);
}
