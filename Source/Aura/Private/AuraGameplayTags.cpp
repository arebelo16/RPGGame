// Andre Rebelo


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitNativeGameplayTags()
{

	/*
	 * Primary Attributes
	 */
	
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
FString("Increases Physical Damage"));
	
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
FString("Increases Magical Damage"));
	
	GameplayTags.Attributes_Primary_Dexterity= UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Dexterity"),
FString("Increases Armor Penetration"));

	GameplayTags.Attributes_Primary_Resilience= UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
FString("Increases Armor"));

	GameplayTags.Attributes_Primary_Vigor= UGameplayTagsManager::Get()
	.AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
FString("Increases Health"));
	
	/*
	* Sencondary Attributes
	 */
	
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),
FString("Reduces damage taken, improves block chance"));

	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),
FString("Ignored % of enemy armor, increases critical hit chance"));

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
FString("Chance to cut incoming damage in half"));

	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
FString("Chance to cut incoming damage in half"));
	
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),
FString("Chance to double damage plus critical hit bonus"));

	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),
FString("Bonus damage added to critical hit"));

	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),
FString("Reduces critical hit chance of attacking enemies"));

	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),
FString("Amount of Health Regenerated every 1 sec"));

	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),
FString("Amount of Mana Regenerated every 1 sec"));

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
FString("Amount of health obtainable"));

	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get()
		.AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
FString("Amount of mana obtainable"));

	/*
	* Input Tags
	 */
	
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),
FString("Input tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),
FString("Input tag for Right Mouse Button"));
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),
FString("Input tag for number 1 key"));
		
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),
FString("Input tag for number 2 key"));
		
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),
FString("Input tag for number 3 key"));
		
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),
FString("Input tag for number 4 key"));

	/*
	 * Damage
	 */

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),
FString("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),
FString("Fire Damage Type"));

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),
FString("Lightning Damage Type"));

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),
FString("Arcane Damage Type"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),
FString("Physical Damage Type"));
	
	/*
	 * Resistances
	 */

	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
FString("Resistance to Arcane Damage Type"));

	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
FString("Resistance to Fire Damage Type"));

	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),
FString("Resistance to Lightning Damage Type"));

	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
FString("Resistance to Physical Damage Type"));

	/*
	* Map of Damage Types to Resistances
	*/

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
		
	/*
	 * Effects
	 */
	
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),
FString("Tag Granted when hit reacting"));

	/*
	* Abilities
	*/
	
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"),
FString("Attack ability tag"));

	/*
	* Montage
	*/	
	
	GameplayTags.Montage_Attack_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Weapon"),
FString("Montage Weapon"));

	GameplayTags.Montage_Attack_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.RightHand"),
FString("Montage Right Hand"));

	GameplayTags.Montage_Attack_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.LeftHand"),
FString("Montage Left Hand"));
}
