#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Skill/Skill.h"

#include "SkillSpawner.generated.h"

class UProjectileMovementComponent;
class USoundBase;

UCLASS()
class TOONTANKS_API ASkillSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillSpawner();

	virtual void Tick(float DeltaTime) override;

	void SetSkillData(FSkillData SD);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovementComponent;

	FSkillData SkillData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ASkill> SkillClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
