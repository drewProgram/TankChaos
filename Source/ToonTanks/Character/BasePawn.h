// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAttributesComponent;
struct FInputActionValue;
class UCapsuleComponent;
class USpringArmComponent;
class UHealthComponent;
class USoundBase;
class UAttributesComponent;

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleDestruction();

	UFUNCTION(BlueprintCallable)
	UAttributesComponent* GetAttributesComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void SetBuggedVFX();

	UFUNCTION(BlueprintImplementableEvent)
	void SetChilledVFX();

	UFUNCTION(BlueprintCallable)
	USceneComponent* GetProjectileSpawnPoint();

	UFUNCTION(BlueprintCallable)
	UStaticMeshComponent* GetTurretMeshComponent();

	void SetTurretRotation(FRotator Rotator);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Shoot();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere)
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UAttributesComponent* AttributesComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;


	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;

};