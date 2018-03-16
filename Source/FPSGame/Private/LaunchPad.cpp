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

	ArrowDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	ArrowDecal->DecalSize = FVector(125.0f);
	ArrowDecal->SetupAttachment(RootComponent);

	LaunchPadParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("LaunchPadParticles"));
	LaunchPadParticles->SetAutoActivate(false);
	LaunchPadParticles->SetupAttachment(RootComponent);

	launchStrength = 1000.0f;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaunchPad::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor);

	FVector launchVector = GetActorForwardVector()*launchStrength;
	launchVector.Z = launchStrength;

	if (MyCharacter != nullptr)
	{
		MyCharacter->LaunchCharacter(launchVector, true, true);
	}
	else
	{
		UPrimitiveComponent* primitive = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
		if (primitive)
		{
			primitive->AddForce(launchVector * launchStrength * 55);
		}
	}

	LaunchPadParticles->ActivateSystem(true);
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

