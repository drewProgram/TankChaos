#include "TankNPC.h"

#include "Kismet/GameplayStatics.h"

ATankNPC::ATankNPC()
{

}

UBehaviorTree* ATankNPC::GetBehaviorTree() const
{
	return Tree;
}

void ATankNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();

	FVector ToTarget = PlayerLocation - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	SetTurretRotation(LookAtRotation);
}
