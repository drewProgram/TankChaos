// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameplayTagContainer.h"

#include "Projectile.generated.h"

class UStaticMeshComponent;
class UProjectileMovementComponent;
class USoundBase;
class ULegacyCameraShake;
class UCameraShakeBase;

UCLASS()
class TOONTANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ApplyCustomTags(FGameplayTagContainer TagContainer);

	UFUNCTION()
	void SetStats(float DmgModifier);

	UFUNCTION()
	FGameplayTag GetDamageType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	float Damage;

	UPROPERTY(EditAnywhere)
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly)
	float DamageModifier;

	FGameplayTag DamageType;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UCameraShakeBase* HitCameraShake;
};
