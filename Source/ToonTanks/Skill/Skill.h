// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GameplayTagContainer.h"

#include "Skill.generated.h"

// TODO: change raw pointer to shared pointer
//DECLARE_DELEGATE_OneParam(FSkillNotify, AActor*);

class ATank;
class UCapsuleComponent;

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
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FGuid PassiveId;

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	int32 MaxUses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASkill> SkillClass;

	FOnSkillStartedDelegate OnSkillStarted;
	FOnSkillCastingDelegate OnSkillCasting;
	FOnSkillEndedDelegate OnSkillEnded;

	UPROPERTY()
	ATank* Owner;

	bool HasSkillEnded;

	int32 GetUsesLeft();

	bool RequestCastSkill(FVector SpawnLocation, float Range, UWorld* WorldRef);

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



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* SkillMesh;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UCapsuleComponent* CapsuleCollider;
};
