#include "BeamSkill.h"

#include "Components/CapsuleComponent.h"

ABeamSkill::ABeamSkill()
{
	SkillCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Skill Collider"));
	SkillCollider->SetupAttachment(RootComponent);
}
