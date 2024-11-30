// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Attributes/Passives.h"

#include "PUSpawner.generated.h"

UCLASS()
class TOONTANKS_API APUSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APUSpawner();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EPassiveType GeneratedPassiveType;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthModifier;

	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthDuration;

	UPROPERTY(EditAnywhere, Category = "Fire Rate")
	float FireRateModifier;

	UPROPERTY(EditAnywhere, Category = "Fire Rate")
	float FireRateDuration;

	UPROPERTY(EditAnywhere, Category = "Movement Speed")
	float MovementSpeedModifier;

	UPROPERTY(EditAnywhere, Category = "Movement Speed")
	float MovementSpeedDuration;

	UPROPERTY(EditAnywhere, Category = "Damage Physical")
	float DamagePhysicalModifier;

	UPROPERTY(EditAnywhere, Category = "Damage Physical")
	float DamagePhysicalDuration;

	UPROPERTY(EditAnywhere, Category = "Damage Lightning")
	float DamageLightningModifier;

	UPROPERTY(EditAnywhere, Category = "Damage Lightning")
	float DamageLightningDuration;

	UPROPERTY(EditAnywhere, Category = "Damage Ice")
	float DamageIceModifier;

	UPROPERTY(EditAnywhere, Category = "Damage Ice")
	float DamageIceDuration;

	void SpawnPowerUp();
	void RandomizePowerUp(struct FPassive& Passive);

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class APowerUp> PowerUpClass;
};
