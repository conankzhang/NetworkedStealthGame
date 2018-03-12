// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComp->bGenerateOverlapEvents = false;

	RootComponent = MeshComp;

	DestroySphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphere"));
	DestroySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroySphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DestroySphere->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	DestroySphere->SetupAttachment(MeshComp);
	DestroySphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnOverlapBegin);

	AreaOfEffectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaOfEffectSphere"));
	AreaOfEffectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaOfEffectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AreaOfEffectSphere->SetupAttachment(MeshComp);

	blackHoleStrength = -5000000;
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
}

void ABlackHole::AffectOverlappedActors()
{
	TSet<AActor *> ActorsToEffect;
	AreaOfEffectSphere->GetOverlappingActors(ActorsToEffect);
	for(AActor * actor : ActorsToEffect)
	{
		UPrimitiveComponent * primitive = Cast<UPrimitiveComponent>(actor->GetRootComponent());
		if (primitive)
		{
			primitive->AddRadialForce(GetActorLocation(), AreaOfEffectSphere->GetScaledSphereRadius(), blackHoleStrength, ERadialImpulseFalloff::RIF_Linear);
		}
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AffectOverlappedActors();
}

void ABlackHole::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != this)
	{
		OtherActor->Destroy();
	}
}