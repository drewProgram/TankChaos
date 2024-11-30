#include "Tower.h"
#include "Tank.h"
#include "../Attributes/AttributesComponent.h"
#include "../Attributes/Passives.h"
#include "../TTGameplayTags.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATower::ATower()
	: FireRange(600.f), bCanShoot(true)
{
	Bory = 0;
}

void ATower::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	AttributesComponent->OnStatusApplied.AddUObject(this, &ATower::HandleStatusApplied);
	AttributesComponent->OnStatusRemoved.AddUObject(this, &ATower::HandleStatusRemoved);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// find dist to tank
	if (CheckTankIsInRange())
	{
		Turn(Tank->GetActorLocation());
		if (bCanShoot)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can shoot"));
			Shoot();
			bCanShoot = false;
			GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::RemoveCooldown, AttributesComponent->GetFireRate(), false);
		}
	}

}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}

void ATower::Turn(FVector Target)
{
	FVector ToTarget = Target - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	TurretMesh->SetWorldRotation(LookAtRotation);
}

void ATower::HandleStatusApplied(FGameplayTag StatusTag, float Duration)
{
	if (StatusTag.MatchesTagExact(TTGameplayTags::Status_Bugged))
	{
		UE_LOG(LogTemp, Warning, TEXT("Stun effect!!!"));
	}
	bCanShoot = false;
}

void ATower::HandleStatusRemoved()
{
	UE_LOG(LogTemp, Warning, TEXT("Back to normal"));
	RemoveCooldown();
}

void ATower::RemoveCooldown()
{
	if (AttributesComponent->GetStatusPassive().PassiveType.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tchau, estou bugado"));
		return;
	}
	bCanShoot = true;
	UE_LOG(LogTemp, Warning, TEXT("Posso atirar de novo"));
}

bool ATower::CheckTankIsInRange()
{
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
		// check to see if tank is in range
		if (Distance <= FireRange)
		{
			return true;
		}
	}

	return false;
}