// Andre Rebelo

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * Singleton
 */
struct FAuraGameplayTags
{
public:
    static const FAuraGameplayTags& Get(){return GameplayTags;}
    static void InitNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Dexterity;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
    FGameplayTag Attributes_Secondary_Armor;
    FGameplayTag Attributes_Secondary_ArmorPenetration;
    FGameplayTag Attributes_Secondary_BlockChance;
    FGameplayTag Attributes_Secondary_CriticalHitChance;
    FGameplayTag Attributes_Secondary_CriticalHitDamage;
    FGameplayTag Attributes_Secondary_CriticalHitResistance;
    FGameplayTag Attributes_Secondary_HealthRegeneration;
    FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;

	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	
	FGameplayTag Effects_HitReact;
	
private:
    static FAuraGameplayTags GameplayTags;
};