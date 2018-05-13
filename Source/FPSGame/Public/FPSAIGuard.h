// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Navigation/PathFollowingComponent.h"

#include "FPSAIGuard.generated.h"

struct FAIRequestID;
class UPawnSensingComponent;
class ATargetPoint;
class AAIController;

UENUM(BlueprintType)
enum class EAIState: uint8
{
	Idle,

	Suspicious,

	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	AAIController* AIController;

	UPROPERTY(EditAnywhere, Category = "Movement")
	TArray<ATargetPoint*> targetPoints;

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	FRotator OriginalRotation;
	FTimerHandle TimerHandle_ResetOrientation;

	UFUNCTION()
	void ResetOrientation();

	EAIState GuardState;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	uint8 currentTargetPoint;

	void MoveToNextTargetPoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
