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

	RootComponent = MeshComp;

	DestroySphere = CreateDefaultSubobject<USphereComponent>(TEXT("DestroySphere"));
	DestroySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DestroySphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	DestroySphere->SetupAttachment(MeshComp);
	DestroySphere->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OnOverlapBegin);

	AreaOfEffectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaOfEffectSphere"));
	AreaOfEffectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AreaOfEffectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	AreaOfEffectSphere->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TSet<AActor *> ActorsToEffect;
	AreaOfEffectSphere->GetOverlappingActors(ActorsToEffect);
}

void ABlackHole::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//OtherActor->Destroy();
}