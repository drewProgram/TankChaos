// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TOONTANKS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnEnemies();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABasePawn> EnemyClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ATank> BossClass;

	void RandomizeAndSpawn(class UNavigationSystemV1* NavSystem, FVector OriginLocation);

	UFUNCTION(BlueprintCallable)
	void SpawnBoss();
};
