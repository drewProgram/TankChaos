#include "TankNPC.h"

#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/WidgetComponent.h"

#include "../Attributes/AttributesComponent.h"
#include "../Core/BaseAIController.h"
#include "../TTGameplayTags.h"

ATankNPC::ATankNPC()
{
	SearchRadius = 1500.f;

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement Component"));
	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Bar"));
	HealthBar->SetupAttachment(BaseMesh);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/Blueprints/Widgets/WBP_NPCHealthBar") };
	if (WidgetClass.Succeeded())
	{
		HealthBar->SetWidgetClass(WidgetClass.Class);
	}
	UE_LOG(LogTemp, Warning, TEXT("Boss constructor called"));
	if (SkillDataObj == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillData nullptr"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillData valid"));
	}
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

	ToTarget = LocationToMove - GetActorLocation();
	LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
	SetActorRotation(LookAtRotation);
}

FVector ATankNPC::GetRandomLocation()
{
	FindRandomLocation();
	return LocationToMove;
}

void ATankNPC::HandleDestruction()
{
	Super::HandleDestruction();

	Destroy();
}

void ATankNPC::BeginPlay()
{
	Super::BeginPlay();

	GetAttributesComponent()->OnHealthUpdated.AddDynamic(this, &ATankNPC::HandleHealthBarUpdate);

	AIController = Cast<ABaseAIController>(GetController());

	FindRandomLocation();
	AIController->MoveToLocation(LocationToMove);

	SetSkill(TTGameplayTags::Skill_Laser, TTGameplayTags::SkillNature_Beam, 5.f, FGuid::NewGuid());

	SkillDataObj->SkillData.MaxUses = 10;
	SkillDataObj->SkillData.UpdateSkillCount();
	SkillDataObj->SkillData.Damage = 1.f;

	bCanShoot = true;
}

void ATankNPC::FindRandomLocation()
{
	if (AIController)
	{
		auto const Origin = GetActorLocation();

		if (auto* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
		{
			FNavLocation Location;
			if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
			{
				LocationToMove = Location.Location;
			}
		}
	}
}

void ATankNPC::RequestShoot()
{
	if (bCanShoot)
	{
		FGameplayTagContainer StatesToCheck;
		StatesToCheck.AddTag(TTGameplayTags::State_CastingSkill);
		StatesToCheck.AddTag(TTGameplayTags::State_StartingCastSkill);
		StatesToCheck.AddTag(TTGameplayTags::State_EndingCastSkill);

		if (TagContainer.HasAnyExact(StatesToCheck))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't shoot now"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("Can shoot"));
		Shoot();
		bCanShoot = false;
		GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATankNPC::RemoveCooldown, AttributesComponent->GetFireRate(), false);
	}
}

void ATankNPC::RemoveCooldown()
{
	if (AttributesComponent->GetStatusPassive())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tchau, estou bugado"));
		return;
	}
	bCanShoot = true;
	UE_LOG(LogTemp, Warning, TEXT("Posso atirar de novo"));
}