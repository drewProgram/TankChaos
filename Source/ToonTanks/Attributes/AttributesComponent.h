// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "Passives.h"

#include "AttributesComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatusAppliedDelegate, FGameplayTag, float);
DECLARE_MULTICAST_DELEGATE(FOnStatusRemovedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActorTookDamageDelegate);
DECLARE_MULTICAST_DELEGATE(FOnActorDiedDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPassiveAddedDelegate, FPassive);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPassiveRemovedDelegate, FPassive);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOONTANKS_API UAttributesComponent : public UActorComponent
{
	friend struct FPassive;

	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributesComponent();

	FOnStatusAppliedDelegate OnStatusApplied;
	FOnStatusRemovedDelegate OnStatusRemoved;

	FOnPassiveAddedDelegate OnPassiveAdded;
	FOnPassiveRemovedDelegate OnPassiveRemoved;

	FOnActorDiedDelegate OnActorDied;

	UPROPERTY(BlueprintAssignable)
	FOnActorTookDamageDelegate OnHealthUpdated;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* Getters and Setters for Attributes and Modifiers
	   Note: Updates for modifiers will be divided by 100
	*/

	UFUNCTION(BlueprintCallable)
	int32 GetHealth() const;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxHealth() const;

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

	FPassive* GetElementalDamage();

	UFUNCTION(BlueprintCallable)
	void UpdateMovementSpeedModifier();

	FPassive* GetStatusPassive();
	FPassive* GetElementalPassive();

	// Expose stacks to HUD
	UFUNCTION(BlueprintCallable)
	int32 GetTotalMovementSpeedStack();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalHealthStack();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalDamageStack();

	UFUNCTION(BlueprintCallable)
	int32 GetTotalFireRateStack();

	FGuid AddPassive(FPassive Passive);

	void RemovePassive(FGuid Id);
	void RemovePassives();

	UFUNCTION(BlueprintCallable)
	bool DoesPassiveAlreadyExists(FPassive Passive);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passives")
	FPassiveStack HealthStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passives")
	FPassiveStack MovementSpeedStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passives")
	FPassiveStack FireRateStack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Passives")
	FPassiveStack DamageStack;

private:
	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	int32 BaseHealth;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetMaxHealth, Category = "Base Attributes")
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintGetter = GetHealth ,Category = "Attributes")
	int32 Health;
	float HealthModifier;

	// RPM
	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	float BaseFireRate;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float FireRate;
	float FireRateModifier;

	UPROPERTY(EditAnywhere, Category = "Base Attributes")
	float BaseMovementSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Attributes")
	float MovementSpeed;
	float MovementSpeedModifier;

	float DamageModifier;

	FPassive* StatusPassive;

	FPassive* ElementalPassive;

	UPROPERTY(EditAnywhere, Category = "Passives")
	TArray<FPassive> Passives;

	void InitPassives();

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);
};
