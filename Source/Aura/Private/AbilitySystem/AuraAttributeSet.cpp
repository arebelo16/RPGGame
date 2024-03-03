// Andre Rebelo


#include "AbilitySystem/AuraAttributeSet.h"

#include <functional>

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if(Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if(Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	//Source = causer of effect
	//Target = target of effect
	SetEffectProperties(Data);
	
	
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data)
{
	const FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	SourceProperties.AbilitySystemComponent = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(SourceProperties.AbilitySystemComponent) && SourceProperties.AbilitySystemComponent->AbilityActorInfo.IsValid()
		&& SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceProperties.AvatarActor = SourceProperties.AbilitySystemComponent->AbilityActorInfo->AvatarActor.Get();
		SourceProperties.Controller = SourceProperties.AbilitySystemComponent->AbilityActorInfo->PlayerController.Get();
		if (SourceProperties.Controller == nullptr && SourceProperties.AvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(SourceProperties.AvatarActor))
			{
				SourceProperties.Controller = Pawn->GetController();
			}
		}

		if (SourceProperties.Controller)
		{
			SourceProperties.Character = Cast<ACharacter>(SourceProperties.Controller->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetProperties.AvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetProperties.Controller = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetProperties.Character = Cast<ACharacter>(TargetProperties.AvatarActor);
		TargetProperties.AbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetProperties.AvatarActor);
	}
}

