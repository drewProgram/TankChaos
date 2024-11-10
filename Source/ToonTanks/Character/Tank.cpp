#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraShakeBase.h"

#include "../TTGameplayTags.h"

ATank::ATank()
	: TurnRate(200.f)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(TurretMesh);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = false; // Do not rotate the arm based on the controller

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SkillData.Owner = this;
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(
		ECollisionChannel::ECC_Visibility,
			false,
			HitResult
		);

		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			25.f,
			12,
			FColor::Red,
			false,
			-1.f
		);

		// TODO: find a way to turret mesh face the same direction that the camera

	}

	if (TagContainer.HasTagExact(TTGameplayTags::State_EndingCastSkill))
	{
		TagContainer.RemoveTag(TTGameplayTags::State_EndingCastSkill);
		SkillData.SkillType = FGameplayTag::EmptyTag;
		UE_LOG(LogTemp, Display, TEXT("Skill ended"));
	}
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ATank::GetSkillData(FSkillData& SD)
{
	SD = SkillData;
}

FVector ATank::GetTurretLookDirection()
{
	return TurretMesh->GetForwardVector();
}

void ATank::BeginPlay()
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

// NOTE: APawn class does not have implementation to the AddMovementInput function. This function does nothing! Use AddActorLocalOffset
void ATank::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		FVector DeltaLocation = FVector::ZeroVector;
		DeltaLocation.X = MovementVector.Y * DeltaTime * MoveSpeed;
		DeltaLocation.Y = MovementVector.X * DeltaTime * MoveSpeed;
		AddActorLocalOffset(DeltaLocation, true);
	}
}

void ATank::TurnTurret(const FInputActionValue& Value)
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

void ATank::TurnTank(const FInputActionValue& Value)
{
	float TurnValue = Value.Get<float>();

	if (Controller != nullptr) 
	{
		FRotator DeltaRotation = FRotator::ZeroRotator;
		double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(GetWorld());

		DeltaRotation.Yaw = TurnValue * DeltaTime * TurnRate;
		AddActorLocalRotation(DeltaRotation);
		TurretMesh->AddLocalRotation(DeltaRotation * -1);
	}
}

void ATank::ShootSpecial()
{
	if (SkillData.SkillClass)
	{
		UE_LOG(LogTemp, Display, TEXT("Shooting special"));

		FGameplayTagContainer StatesToCheck;
		StatesToCheck.AddTag(TTGameplayTags::State_CastingSkill);
		StatesToCheck.AddTag(TTGameplayTags::State_StartingCastSkill);
		StatesToCheck.AddTag(TTGameplayTags::State_EndingCastSkill);

		if (TagContainer.HasAnyExact(StatesToCheck))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't use special now"));
			return;
		}
		TagContainer.AddTag(TTGameplayTags::State_StartingCastSkill);

		if (SkillData.RequestCastSkill(ProjectileSpawnPoint->GetComponentLocation(), 1000.f, GetWorld()))
		{
			TagContainer.RemoveTag(TTGameplayTags::State_StartingCastSkill);
			TagContainer.AddTag(TTGameplayTags::State_CastingSkill);
		}
		else
		{
			TagContainer.RemoveTag(TTGameplayTags::State_StartingCastSkill);
		}
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);

		EnhancedInputComponent->BindAction(TurnTurretAction, ETriggerEvent::Triggered, this, &ATank::TurnTurret);

		EnhancedInputComponent->BindAction(TurnTankAction, ETriggerEvent::Triggered, this, &ATank::TurnTank);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ATank::Shoot);

		EnhancedInputComponent->BindAction(ShootSpecialAction, ETriggerEvent::Triggered, this, &ATank::ShootSpecial);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."));
	}
}
