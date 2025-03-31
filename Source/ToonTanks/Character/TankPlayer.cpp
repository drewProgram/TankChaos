#include "TankPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"

#include "../Attributes/AttributesComponent.h"
#include "../TTGameplayTags.h"
#include "../Weapons/Projectile.h"

ATankPlayer::ATankPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(TurretMesh);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Do not rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	bCanShoot = true;
}

void ATankPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATankPlayer::Move);

		EnhancedInputComponent->BindAction(TurnTurretAction, ETriggerEvent::Triggered, this, &ATankPlayer::TurnTurret);
		
		EnhancedInputComponent->BindAction(TurnTankAction, ETriggerEvent::Triggered, this, &ATankPlayer::TurnTank);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATankPlayer::Shoot);

		EnhancedInputComponent->BindAction(ShootSpecialAction, ETriggerEvent::Triggered, this, &ATankPlayer::ShootSpecial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
	}
}

void ATankPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.Z = 85.f;
		SetActorLocation(SpawnLocation);
	}
}

void ATankPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MovementVector.Y * DeltaTime * AttributesComponent->GetMovementSpeed();
		DeltaLocation.Y = MovementVector.X * DeltaTime * AttributesComponent->GetMovementSpeed();
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void ATankPlayer::TurnTurret(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		FRotator DeltaRotation = FRotator::ZeroRotator;
		double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		DeltaRotation.Yaw = LookAxisVector.X * DeltaTime * TurnRate;
		CameraBoom->AddLocalRotation(DeltaRotation);
		TurretMesh->AddLocalRotation(DeltaRotation);
	}
}

void ATankPlayer::TurnTank(const FInputActionValue& Value)
{
	float TurnValue = Value.Get<float>();

	if (Controller != nullptr)
	{
		FRotator DeltaRotation = FRotator::ZeroRotator;
		double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		DeltaRotation.Yaw = (TurnValue * 1.5) * TurnRate * DeltaTime;
		AddActorLocalRotation(DeltaRotation);
		TurretMesh->AddLocalRotation(DeltaRotation * -1);
	}
}

void ATankPlayer::Shoot()
{
	if (bCanShoot)
	{
		FVector Location = ProjectileSpawnPoint->GetComponentLocation();
		FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();

		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
		if (Projectile)
		{
			Projectile->SetOwner(this);
			AActor* PO = Projectile->GetOwner();
			if (PO != nullptr)
			{
				Projectile->SetStats(AttributesComponent->GetDamageModifier());
				FGameplayTagContainer ProjectileTags;
				if (!AttributesComponent->GetElementalDamage())
				{
					UE_LOG(LogTemp, Warning, TEXT("Don't have elemental ammo. Normal damage"));
					ProjectileTags.AddTag(TTGameplayTags::Damage_Physical);
					Projectile->ApplyCustomTags(ProjectileTags);
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("Passive: %s"), *AttributesComponent->GetElementalDamage()->PassiveType.ToString());
					ProjectileTags.AddTag(AttributesComponent->GetElementalDamage()->PassiveType);
					Projectile->ApplyCustomTags(ProjectileTags);
				}

				bCanShoot = false;
				GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATankPlayer::RemoveCooldown, AttributesComponent->GetFireRate(), false);
			}
		}

	}
}

void ATankPlayer::RemoveCooldown()
{
	bCanShoot = true;
}
