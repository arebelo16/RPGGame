// Andre Rebelo


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetDrawDebug(true);
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	
	Autorun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		// When attached the Damage text animation will be played right away, so we detach right after attaching it
		// This to prevent the Damage text following the TargetCharacter
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::Autorun()
{
	if (!Spline || !GetPawn() || !bAutoRunning) return;

	FVector PawnLocation = GetPawn()->GetActorLocation();
	PawnLocation.Z = 0;

	if (!bRunningToNextPoint)
	{
		for (int32 i = 1; i < Spline->GetNumberOfSplinePoints(); ++i)
		{
			FVector PointActualLocation = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
			if (!ReachedDestinations.Contains(PointActualLocation)){
				if (FVector::Distance(PawnLocation, PointActualLocation) > AutoRunAcceptanceRadius)
				{
					DrawDebugSphere(GetWorld(), PointActualLocation, 8.f, 8, FColor::Red, false, 5.f);
					FVector Direction = (PointActualLocation - PawnLocation).GetSafeNormal();
					GetPawn()->AddMovementInput(Direction);
					NextDestination = PointActualLocation;
					bRunningToNextPoint = true;
					return;
				}
			}
			
		}

		bAutoRunning = false;
	}
	else
	{
		if (FVector::Distance(PawnLocation, NextDestination) <= AutoRunAcceptanceRadius)
		{
			bRunningToNextPoint = false;
			ReachedDestinations.Add(NextDestination);
		}
		else
		{
			FVector Direction = (NextDestination - PawnLocation).GetSafeNormal();
			GetPawn()->AddMovementInput(Direction);
		}
	}
}





void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	if(!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	if (LastActor != ThisActor) {
		if (LastActor != nullptr) {
			LastActor->UnHighlightActor();
			LastActor->HideHealthBar();
		}
		if (ThisActor != nullptr) {
			ThisActor->HighlightActor();
			ThisActor->ShowHealthBar();
		}
		LastActor = ThisActor;
	}

}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}

}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
	
	if (!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressedThreshold && ControlledPawn)
		{
			UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination);
			if (NavigationPath && NavigationPath->IsValid())
			{
				// Clear spline and add points along the path
				Spline->ClearSplinePoints(true);
				ReachedDestinations.Reset();
				for (const FVector& PointLoc : NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World, true);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				
				// Update CachedDestination and enable autorun
				if (NavigationPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavigationPath->PathPoints.Last();
					bAutoRunning = true;
					bRunningToNextPoint = false;
				};
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

		if(APawn* ControledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControledPawn->GetActorLocation()).GetSafeNormal();
			ControledPawn->AddMovementInput(WorldDirection);
		}
	}
	
	//if(GetAbilitySystemComponent() == nullptr) return;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetAbilitySystemComponent()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
	
}


