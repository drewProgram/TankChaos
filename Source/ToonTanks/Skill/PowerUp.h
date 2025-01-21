// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Attributes/AttributesComponent.h"

#include "PowerUp.generated.h"

class BasePawn;

UCLASS()
class TOONTANKS_API APowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerUp();

	virtual void Tick(float DeltaTime) override;

	void SetPassive(FPassive P);

	UFUNCTION(BlueprintImplementableEvent)
	void SetMesh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float PitchValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float YawValue;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RollValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PUMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FPassive Passive;

	bool bAlreadySet;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
