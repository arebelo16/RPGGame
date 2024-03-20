// Andre Rebelo

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UAttributeSet;
class UAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Combat Interface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AAuraCharacterBase* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	// End Combat Interface
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
protected:
	virtual void BeginPlay() override;

	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category= "Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category= "Combat")
	FName LeftHandTipSocketName;

	UPROPERTY(EditAnywhere, Category= "Combat")
	FName RightHandTipSocketName;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitDefaultAttributes() const;

	void AddCharacterAbilities();

	// Dissolve Effects

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstace);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstace);
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category= "Combat")
	USoundBase* DeathSound;

private:
	
	UPROPERTY(EditAnywhere, Category= "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

};
