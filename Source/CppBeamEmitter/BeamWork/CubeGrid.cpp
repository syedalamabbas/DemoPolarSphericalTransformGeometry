// Fill out your copyright notice in the Description page of Project Settings.

#include "CppBeamEmitter.h"
#include "CubeGrid.h"


// Sets default values
ACubeGrid::ACubeGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnRoot"));
	RootComponent = WhereToSpawn;

	BeamParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DefaultEmptyParticleSystem"));
	BeamParticleSystem->SetupAttachment(RootComponent);

	// Loading some grid defaults
	N = 9;
	GridSpacing = 10;
}

// Called when the game starts or when spawned
void ACubeGrid::BeginPlay()
{
	Super::BeginPlay();
	ConstructGrid();
}

// Called every frame
void ACubeGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeGrid::ConstructGrid()
{
	FVector BoxLocation = WhereToSpawn->GetComponentLocation();
	FVector BeamSourcePoint;

	UParticleSystem* TemplateParticleSystem = BeamParticleSystem->Template;
	UParticleSystemComponent* CurrentEmitter = NULL;
	FVector BeamTargetPoint;

	int32 lower = -(N - 1) / 2;
	int32 upper = (N - 1) / 2;

	for (int i = lower; i <= upper; i++)
	{
		for (int j = lower; j <= upper; j++)
		{
			if (BeamParticleSystem)
			{
				// Length is X - axis
				BeamSourcePoint.X = BoxLocation.X + lower*GridSpacing;                     // Constant
				BeamSourcePoint.Y = BoxLocation.Y + j*GridSpacing;     // Moving in Straight Line
				BeamSourcePoint.Z = BoxLocation.Z + i*GridSpacing;     // Moving in Straight Line

				BeamTargetPoint = FVector(BeamSourcePoint);
				BeamTargetPoint.X = BeamTargetPoint.X + (N-1)*GridSpacing;  // Beam length in X direction 

				// Spawning
				CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticleSystem->Template,
					BeamSourcePoint,
					FRotator::ZeroRotator,
					true);

				CurrentEmitter->SetBeamSourcePoint(0, BeamSourcePoint, 0);
				CurrentEmitter->SetBeamTargetPoint(0, BeamTargetPoint, 0);


				// Length is Y - axis
				BeamSourcePoint.Y = BoxLocation.Y + lower*GridSpacing;                      // Constant
				BeamSourcePoint.X = BoxLocation.X + j*GridSpacing;     // Moving in Straight Line
				BeamSourcePoint.Z = BoxLocation.Z + i*GridSpacing;     // Moving in Straight Line

				BeamTargetPoint = FVector(BeamSourcePoint);
				BeamTargetPoint.Y = BeamTargetPoint.Y + (N - 1)*GridSpacing;  // Beam length in Y direction 

				// Spawning
				CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					BeamParticleSystem->Template,
					BeamSourcePoint,
					FRotator::ZeroRotator,
					true);

				CurrentEmitter->SetBeamSourcePoint(0, BeamSourcePoint, 0);
				CurrentEmitter->SetBeamTargetPoint(0, BeamTargetPoint, 0);


				// Length is Z - axis
				BeamSourcePoint.Z = BoxLocation.Z + lower*GridSpacing;                     // Constant
				BeamSourcePoint.X = BoxLocation.X + j*GridSpacing;     // Moving in Straight Line
				BeamSourcePoint.Y = BoxLocation.Y + i*GridSpacing;     // Moving in Straight Line

				BeamTargetPoint = FVector(BeamSourcePoint);
				BeamTargetPoint.Z = BeamTargetPoint.Z + (N - 1)*GridSpacing;  // Beam length in Z direction 

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

	FVector TargetMeshLocation = FVector(BoxLocation);

	for (int i = lower; i <= upper; i++)
	{
		for (int j = lower; j <= upper; j++)
		{
			for (int k = lower; k <= upper; k++)
			{
				if (NULL != WhatToSpawn)
				{
					UWorld* CurrentWorld = GetWorld();
					if (NULL != CurrentWorld)
					{

						TargetMeshLocation.X = BoxLocation.X + i*GridSpacing;
						TargetMeshLocation.Y = BoxLocation.Y + j*GridSpacing;
						TargetMeshLocation.Z = BoxLocation.Z + k*GridSpacing;

						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;
						SpawnParams.Instigator = Instigator;
						GetWorld()->SpawnActor<AActor>(WhatToSpawn, TargetMeshLocation, FRotator::ZeroRotator, SpawnParams);
					}
				}
			}
		}
	}
}
