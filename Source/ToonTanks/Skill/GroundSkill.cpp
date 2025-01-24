#include "GroundSkill.h"

#include "Components/BoxComponent.h"

AGroundSkill::AGroundSkill()
{
	SkillCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Skill Collider"));
	SkillCollider->SetupAttachment(RootComponent);
}
