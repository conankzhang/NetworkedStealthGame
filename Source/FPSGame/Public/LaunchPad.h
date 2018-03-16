// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaunchPad.generated.h"

class UBoxComponent;
class UDecalComponent;
class UParticleSystemComponent;

UCLASS()
class FPSGAME_API ALaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaunchPad();

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	FVector launchVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* LaunchPadCollision;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent* ArrowDecal;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UParticleSystemComponent* LaunchPadParticles;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
