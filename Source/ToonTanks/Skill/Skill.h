// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "GameplayTagContainer.h"

#include "Skill.generated.h"

// TODO: change raw pointer to shared pointer
//DECLARE_DELEGATE_OneParam(FSkillNotify, AActor*);

class ATank;
class UDamageHandlerComponent;
class UShapeComponent;

DECLARE_MULTICAST_DELEGATE(FOnSkillStartedDelegate)
DECLARE_MULTICAST_DELEGATE(FOnSkillCastingDelegate)
DECLARE_MULTICAST_DELEGATE(FOnSkillEndedDelegate)

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	FSkillData();

	UPROPERTY(EditAnywhere)
	FGameplayTag SkillType;

	UPROPERTY(EditAnywhere)
	FGameplayTag SkillNature;

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FGuid PassiveId;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	int32 MaxUses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bSpawnsFromProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASkill> SkillClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASkillSpawner> SkillSpawner;

	FOnSkillStartedDelegate OnSkillStarted;
	FOnSkillCastingDelegate OnSkillCasting;
	FOnSkillEndedDelegate OnSkillEnded;

	UPROPERTY(BlueprintReadOnly)
	ATank* Owner;

	bool HasSkillEnded;

	int32 GetUsesLeft();

	bool RequestCastSkill(FVector SpawnLocation, UWorld* WorldRef, FRotator Rotation = FRotator());

	void NotifySkillEnded();

	void UpdateSkillCount();

private:
	UPROPERTY(VisibleAnywhere)
	int32 UsesLeft;
};

UCLASS()
class TOONTANKS_API ASkill : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkill();

	//FSkillNotify OnSkillEnded;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSkillData SkillData;

	UFUNCTION(BlueprintImplementableEvent)
	void SetupSkillData(FSkillData SD);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	UShapeComponent* SkillCollider;

	UPROPERTY(VisibleAnywhere)
	class UDamageHandlerComponent* DamageHandlerComponent;

	bool bTimerSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Visual", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* SkillParticle;

	UFUNCTION(BlueprintCallable)
	void SetDestroyTimer();

	UFUNCTION()
	void DestroyTest();
};
