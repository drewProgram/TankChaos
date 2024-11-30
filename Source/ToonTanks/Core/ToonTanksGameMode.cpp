#include "ToonTanksGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/Tank.h"
#include "../Character/Tower.h"
#include "../Character/ToonTanksPlayerController.h"

AToonTanksGameMode::AToonTanksGameMode()
	: StartDelay(3.f), TargetTowers(0), CurrentWave(1), TotalEnemies(5)
{

}

void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else if (ATower* Tower = Cast<ATower>(DeadActor))
	{
		Tower->HandleDestruction();
		TargetTowers -= 1;

		if (TargetTowers == 0)
		{
			HandleWaveChange();
		}
	}
}


void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
			PlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(
			TimerHandle,
			TimerDelegate,
			StartDelay,
			false
		);

	}

}

void AToonTanksGameMode::HandleWaveChange()
{
	CurrentWave += 1;

	switch (CurrentWave)
	{
	case 2:
	case 3:
		UE_LOG(LogTemp, Display, TEXT("New wave appearing"));
		EnemyHealthBonus += 10.f;
		EnemyDamageBonus += 5.f;
		TotalEnemies += 5;
		TargetTowers = TotalEnemies;

		break;

	default:
		GameOver(true);
		break;
	}

	OnWaveStarted.ExecuteIfBound();
}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

	return Towers.Num();
}

int32 AToonTanksGameMode::GetTotalEnemies()
{
	return TotalEnemies;
}

float AToonTanksGameMode::GetEnemyHealthBonus()
{
	return EnemyHealthBonus;
}

float AToonTanksGameMode::GetEnemyDamageBonus()
{
	return EnemyDamageBonus;
}
