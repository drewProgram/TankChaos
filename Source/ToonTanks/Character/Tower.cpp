#include "Tower.h"
#include "Tank.h"


#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ATower::ATower()
	: FireRange(600.f), FireRate(2.f)
{

}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// find dist to tank
	if (CheckTankIsInRange())
	{
		Turn(Tank->GetActorLocation());
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

void ATower::CheckFireCondition()
{
	if (CheckTankIsInRange())
	{
		Shoot();
	}
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