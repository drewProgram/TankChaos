#include "ToonTanksGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "../Character/Tank.h"
#include "../Character/Tower.h"
#include "../Character/ToonTanksPlayerController.h"

AToonTanksGameMode::AToonTanksGameMode()
	: StartDelay(3.f), TargetTowers(0)
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
			GameOver(true);
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
		UE_LOG(LogTemp, Display, TEXT("Player Controller not null"));
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

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);

	return Towers.Num();
}
