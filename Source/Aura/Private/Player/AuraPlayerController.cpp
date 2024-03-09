// Andre Rebelo


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
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

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
		&ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

	Autorun();
}

void AAuraPlayerController::Autorun()
{
	if (!Spline || !GetPawn() || !bAutoRunning) return;

	// Find the closest point on the spline
	FVector PawnLocation = GetPawn()->GetActorLocation();
	FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(PawnLocation, ESplineCoordinateSpace::World);
	FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

	// Update movement input
	GetPawn()->AddMovementInput(Direction);

	// Check if reached destination
	float DistanceToDestination = FVector::Distance(LocationOnSpline, CachedDestination);
	if (DistanceToDestination <= AutoRunAcceptanceRadius)
	{
		bAutoRunning = false;
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
		}
		if (ThisActor != nullptr) {
			ThisActor->HighlightActor();
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
	
	if (bTargeting)
	{
		if (GetAbilitySystemComponent()) GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressedThreshold && ControlledPawn)
		{
			UNavigationPath* NavigationPath = UNavigationSystemV1::FindPathToLocationSynchronously(
				this, ControlledPawn->GetActorLocation(), CachedDestination);
			if (NavigationPath && NavigationPath->IsValid())
			{
				// Clear spline and add points along the path
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavigationPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				
				// Update CachedDestination and enable autorun
				if (NavigationPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavigationPath->PathPoints.Last();
					bAutoRunning = true;
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

	if (bTargeting)
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


