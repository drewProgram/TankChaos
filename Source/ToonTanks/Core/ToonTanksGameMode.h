// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWaveStartedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCountdownEnded);

/**
 * 
 */
UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AToonTanksGameMode();

	void ActorDied(AActor* DeadActor);

	int32 GetTargetTowerCount();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalEnemies();

	float GetEnemyHealthBonus();
	float GetEnemyDamageBonus();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWave() const;

	FOnWaveStartedDelegate OnWaveStarted;
	FOnCountdownEnded OnCountdownEnded;
	
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetCurrentWave, Category = "Wave")
	int32 CurrentWave;

private:
	class ATankPlayer* TankPlayer;
	class AToonTanksPlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly)
	float StartDelay;

	void HandleGameStart();

	int32 TargetTowers;
	int32 TotalEnemies;


	UPROPERTY(EditDefaultsOnly, Category = "Global Enemy Attribute Bonus")
	float EnemyHealthBonus;

	UPROPERTY(EditDefaultsOnly, Category = "Global Enemy Attribute Bonus")
	float EnemyDamageBonus;

	void HandleWaveChange();

	UFUNCTION()
	void HandleCountdownEnd(bool bPlayerEnabled);
};
