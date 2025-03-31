#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

#include "ToonTanksGameMode.h"
#include "../Character/BasePawn.h"
#include "../Character/Tank.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->OnWaveStarted.AddUObject(this, &AEnemySpawner::SpawnEnemies);
	}
	
	SpawnEnemies();
}

void AEnemySpawner::RandomizeAndSpawn(UNavigationSystemV1* NavSystem, FVector OriginLocation)
{
	FNavLocation ResultLocation;
	NavSystem->GetRandomReachablePointInRadius(OriginLocation, 5000.f, ResultLocation);

	FVector Location = FVector(ResultLocation.Location.X, ResultLocation.Location.Y, 85.f);
	FRotator Rotation = FRotator::ZeroRotator;

	ABasePawn* Enemy = GetWorld()->SpawnActor<ABasePawn>(EnemyClass, Location, Rotation);
	while (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Trying to spawn again"));
		NavSystem->GetRandomReachablePointInRadius(OriginLocation, 5000.f, ResultLocation);
		Location = FVector(ResultLocation.Location.X, ResultLocation.Location.Y, 85.f);
		Enemy = GetWorld()->SpawnActor<ABasePawn>(EnemyClass, Location, Rotation);
	}
}

void AEnemySpawner::SpawnBoss()
{
	UE_LOG(LogTemp, Display, TEXT("Trying to spawn boss"));
	FVector SpawnLocation = FVector(460.0, -550.0, 100);
	FRotator Rotation = FRotator::ZeroRotator;

	ATank* Enemy = GetWorld()->SpawnActor<ATank>(BossClass, SpawnLocation, Rotation);
	if (Enemy == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not spawn boss"));
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemies()
{
	AToonTanksGameMode* GameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		int32 TotalEnemies = GameMode->GetTotalEnemies();
		FVector OriginLocation = GetActorLocation();
		UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
		for (int i = 1; i <= TotalEnemies; i++)
		{
			if (GameMode->GetCurrentWave() == 5)
			{
				SpawnBoss();
				return;
			}

			RandomizeAndSpawn(NavSystem, OriginLocation);
		}
	}
}

