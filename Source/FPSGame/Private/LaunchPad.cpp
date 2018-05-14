// Fill out your copyright notice in the Description page of Project Settings.

#include "LaunchPad.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	LaunchPadCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LaunchPadCollision"));
	LaunchPadCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LaunchPadCollision->SetupAttachment(RootComponent);
	LaunchPadCollision->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OnOverlapBegin);

	LaunchPadParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaunchPadParticles"));
	LaunchPadParticles->SetAutoActivate(false);
	LaunchPadParticles->SetupAttachment(RootComponent);

	LaunchStrength = 1500.0f;
	LaunchAngle = 45.0f;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaunchPad::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);

	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchAngle;

	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	if (MyCharacter != nullptr)
	{
		MyCharacter->LaunchCharacter(LaunchVelocity, true, true);
		LaunchPadParticles->ActivateSystem(true);
	}
	else if(OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
		LaunchPadParticles->ActivateSystem(true);
	}

}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

