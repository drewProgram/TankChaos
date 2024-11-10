#include "TTGameplayTags.h"

namespace TTGameplayTags {
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute, "Attribute", "Indicates its an attribute");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_Health, "Attribute.Health", "Passive for health");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_MovementSpeed, "Attribute.MovementSpeed", "Passive for Movement Speed");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attribute_FireRate, "Attribute.FireRate", "Passive for Fire Rate");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage, "Damage", "Indicates source of damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Physical, "Damage.Physical", "Normal Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Elemental, "Damage.Elemental", "Indicates elemental damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Elemental_Ice, "Damage.Elemental.Ice", "Passive for Ice Damage");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Damage_Elemental_Lightning, "Damage.Elemental.Lightning", "Passive for Lightning damage");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill, "Skill", "Indicates its a skill");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Acid, "Skill.Acid", "Passive for Acid skill");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Skill_Laser, "Skill.Laser", "Passive for Laser skill");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status, "Status", "Indicates its a status");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Chilled, "Status.Chilled", "Passive for Chilled status");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Status_Bugged, "Status.Bugged", "Passive for Bugged status");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Modifiers, "Modifiers", "Indicates its a modifier");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Modifiers_Increase, "Modifiers.Increase", "Buff");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Modifiers_Decrease, "Modifiers.Decrease", "Debuff");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_StartingCastSkill, "State.StartingCastSkill", "Starting to cast skill");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_CastingSkill, "State.CastingSkill", "Is casting skill");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_EndingCastSkill, "State.EndingCastSkill", "Finishing to cast skill");
}