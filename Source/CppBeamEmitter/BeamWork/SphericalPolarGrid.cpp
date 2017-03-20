// Fill out your copyright notice in the Description page of Project Settings.

#include "CppBeamEmitter.h"
#include "SphericalPolarGrid.h"


// Sets default values
ASphericalPolarGrid::ASphericalPolarGrid()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnRoot"));
	RootComponent = WhereToSpawn;

	BeamParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DefaultEmptyParticleSystem"));
	BeamParticleSystem->SetupAttachment(RootComponent);

	// Loading some grid defaults
	K = 10;
	M = 10;
	N = 9;
	GridSpacing = 10;

}

// Called when the game starts or when spawned
void ASphericalPolarGrid::BeginPlay()
{
	Super::BeginPlay();
	ConstructGrid();
}

// Called every frame
void ASphericalPolarGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASphericalPolarGrid::ConstructGrid()
{
	FVector BoxLocation = WhereToSpawn->GetComponentLocation();
	FVector BeamSourcePoint;

	UParticleSystem* TemplateParticleSystem = BeamParticleSystem->Template;
	UParticleSystemComponent* CurrentEmitter = NULL;
	FVector BeamTargetPoint;

	FVector TargetMeshLocation = FVector(BoxLocation);
	float theta;
	float phi;

	int lower = -(N - 1) / 2;
	int upper = (N - 1) / 2;


	for (int k = 0; k < K; k++)
	{
		for (int m = 0; m < M; m++)
		{
			for (int n = lower; n <= upper; n++)
			{
				if (NULL != WhatToSpawn)
				{
					UWorld* CurrentWorld = GetWorld();
					if (NULL != CurrentWorld)
					{
						theta = k * PI / K;
						phi = m * PI / M;
						TargetMeshLocation.X = BoxLocation.X + n*GridSpacing*FMath::Cos(theta)*FMath::Cos(phi);
						TargetMeshLocation.Y = BoxLocation.Y + n*GridSpacing*FMath::Cos(theta)*FMath::Sin(phi);
						TargetMeshLocation.Z = BoxLocation.Z + n*GridSpacing*FMath::Sin(theta);


						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;
						SpawnParams.Instigator = Instigator;
						GetWorld()->SpawnActor<AActor>(WhatToSpawn, TargetMeshLocation, FRotator::ZeroRotator, SpawnParams);


						if (n == lower)
						{
							BeamSourcePoint = TargetMeshLocation;
						}

						if (n == upper)
						{
							BeamTargetPoint = TargetMeshLocation;
							// Spawning
							CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
								GetWorld(),
								BeamParticleSystem->Template,
								BeamSourcePoint,
								FRotator::ZeroRotator,
								true);

							CurrentEmitter->SetBeamSourcePoint(0, BeamSourcePoint, 0);
							CurrentEmitter->SetBeamTargetPoint(0, BeamTargetPoint, 0);
						}
					}
				}
			}
		}
	}
}