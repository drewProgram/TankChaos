#pragma once

#include "NativeGameplayTags.h"

namespace TTGameplayTags {
    // Passives
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_Health);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_MovementSpeed);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Attribute_FireRate);
    
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Physical);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Ice);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Damage_Elemental_Lightning);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Acid);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Skill_Laser);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillNature_Spawner);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillNature_Beam);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(SkillNature_Ground);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Chilled);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Bugged);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Modifiers);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Modifiers_Increase);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(Modifiers_Decrease);

    UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_StartingCastSkill);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_CastingSkill);
    UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_EndingCastSkill);
}