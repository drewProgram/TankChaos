// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "Passives.h"

#include "AttributesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOONTANKS_API UAttributesComponent : public UActorComponent
{
	friend struct FPassive;

	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributesComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Getters and Setters for Attributes and Modifiers
	   Note: Updates for modifiers will be divided by 100
	*/

	UFUNCTION(BlueprintCallable)
	int32 GetHealth();

	UFUNCTION(BlueprintCallable)
	void UpdateHealthModifier();

	UFUNCTION(BlueprintCallable)
	float GetFireRate();

	UFUNCTION(BlueprintCallable)
	void UpdateFireRateModifier();

	UFUNCTION()
	void UpdateDamageModifier();

	UFUNCTION()
	float GetDamageModifier();

	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed();

	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeedModifier();

	void AddPassive(FPassive Passive);

	void RemovePassive(FGuid Id);

	UFUNCTION(BlueprintCallable)
	bool DoesPassiveAlreadyExists(FPassive Passive);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	int32 BaseHealth;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	int32 Health;
	float HealthModifier;

	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	float FireRate;
	float FireRateModifier;
		
	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	float MovementSpeed;
	float MovementSpeedModifier;

	float DamageModifier;

	UPROPERTY(EditAnywhere, Category = "Passives")
	FPassiveStack HealthStack;

	UPROPERTY(EditAnywhere, Category = "Passives")
	FPassiveStack MovementSpeedStack;

	UPROPERTY(EditAnywhere, Category = "Passives")
	FPassiveStack FireRateStack;

	UPROPERTY(EditAnywhere, Category = "Passives")
	FPassiveStack DamageStack;

	FPassive* StatusPassive;

	FPassive* ElementalPassive;

	UPROPERTY(EditAnywhere, Category  = "Passives")
	TArray<FPassive> Passives;

	void InitPassives();

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
};
