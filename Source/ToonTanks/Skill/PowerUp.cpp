#include "PowerUp.h"

#include "../Character/Tank.h"

// Sets default values
APowerUp::APowerUp()
	: bAlreadySet(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PUMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = PUMesh;
}

// Called when the game starts or when spawned
void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
	PUMesh->OnComponentBeginOverlap.AddDynamic(this, &APowerUp::BeginOverlap);
}

void APowerUp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAlreadySet) return;
	ATank* Player = Cast<ATank>(OtherActor);
	if (Player)
	{
		if (Passive.PassiveType != FGameplayTag::EmptyTag)
		{
			bAlreadySet = true;
			Player->GetAttributesComponent()->AddPassive(Passive);
			UE_LOG(LogTemp, Display, TEXT("Got Power Up %s"), *Passive.PassiveType.ToString());
			Destroy();
		}
	}
}

// Called every frame
void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

