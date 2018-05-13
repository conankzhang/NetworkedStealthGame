// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"
#include "AITypes.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	currentTargetPoint = 0;

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	OriginalRotation = GetActorRotation();

	GuardState = EAIState::Idle;

	AIController = Cast<AAIController>(GetController());
	AIController->ReceiveMoveCompleted.AddDynamic(this, &AFPSAIGuard::OnMoveCompleted);

	MoveToNextTargetPoint();
}

void AFPSAIGuard::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	AFPSGameMode * GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

	SetGuardState(EAIState::Alerted);
}

void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if(GuardState == EAIState::Alerted)
	{
		return; 
	}

	AIController->PauseMove(0);
	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);
}

void AFPSAIGuard::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	MoveToNextTargetPoint();
}

void AFPSAIGuard::ResetOrientation()
{
	if(GuardState == EAIState::Alerted)
	{
		return; 
	}

	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
	AIController->ResumeMove(0);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	OnStateChanged(GuardState);
}

void AFPSAIGuard::MoveToNextTargetPoint()
{
	if(targetPoints.Num() == 0)
	{
		return;
	}

	if(currentTargetPoint >= targetPoints.Num())
	{
		currentTargetPoint = 0;
	}

	AIController->MoveToActor(targetPoints[currentTargetPoint++]);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}