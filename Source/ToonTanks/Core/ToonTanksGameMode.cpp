#include "ToonTanksGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/Tank.h"
#include "../Character/Tower.h"
#include "../Character/TankPlayer.h"
#include "../Character/ToonTanksPlayerController.h"
#include "../Attributes/AttributesComponent.h"

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
	if (ATankPlayer* Actor = Cast<ATankPlayer>(DeadActor))
	{
		TankPlayer->HandleDestruction();
		if (PlayerController)
		{
			PlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
		return;
	}
	else if (ATower* Tower = Cast<ATower>(DeadActor))
	{
		Tower->HandleDestruction();
		TargetTowers -= 1;

		if (TargetTowers == 0)
		{
			HandleWaveChange();
		}

		return;
	}

	ATank* Boss = Cast<ATank>(DeadActor);
	Boss->HandleDestruction();
	HandleWaveChange();
}


void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	TankPlayer = Cast<ATankPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
	PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(false);
		FTimerHandle TimerHandle;
		FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(
			this,
			&AToonTanksGameMode::HandleCountdownEnd,
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
	case 4:
		UE_LOG(LogTemp, Display, TEXT("New wave appearing"));
		EnemyHealthBonus += 10.f;
		EnemyDamageBonus += 5.f;
		TotalEnemies += 5;
		TargetTowers = TotalEnemies;

		break;

	case 5:
		UE_LOG(LogTemp, Warning, TEXT("WARNING: BOSS ROUND"));

		TotalEnemies = 6;
		TargetTowers = 5;

		EnemyDamageBonus += 5.f;
		EnemyHealthBonus += 5.f;

		break;
	default:
		GameOver(true);

		return;
	}

	OnWaveStarted.Broadcast();
}

void AToonTanksGameMode::HandleCountdownEnd(bool bPlayerEnabled)
{
	PlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PlayerController)
	{
		PlayerController->SetPlayerEnabledState(bPlayerEnabled);
	}
	OnCountdownEnded.Broadcast();
	TankPlayer->GetAttributesComponent()->OnHealthUpdated.Broadcast();
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

int32 AToonTanksGameMode::GetCurrentWave() const
{
	return CurrentWave;
}
