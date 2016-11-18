// Fill out your copyright notice in the Description page of Project Settings.

#include "CppBeamEmitter.h"
#include "BeamEmitterBaseActor.h"

// Sets default values
ABeamEmitterBaseActor::ABeamEmitterBaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnRoot"));
	RootComponent = WhereToSpawn;

	BeamParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DefaultEmptyParticleSystem"));
	BeamParticleSystem->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABeamEmitterBaseActor::BeginPlay()
{
	Super::BeginPlay();
	CreateDesiredBeamPattern();
}

// Called every frame
void ABeamEmitterBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ABeamEmitterBaseActor::GetOriginInVolume()
{
	return WhereToSpawn->GetComponentLocation();
}

void ABeamEmitterBaseActor::CreateDesiredBeamPattern()
{
	int32 NumberOfBeams = 3;
	int32 BeamLength = 400;
	int32 ZOffset = 10;

	FVector BoxLocation = GetOriginInVolume();
	FVector TargetPoint;


	for (int32 i = 0; i < NumberOfBeams; i++)  // Along X Axis
	{
		TargetPoint.X = BoxLocation.X;                 // Constant
		TargetPoint.Y = BoxLocation.Y;                 // Constant
		TargetPoint.Z = BoxLocation.Z + i*ZOffset;   // Moving in Straight Line

		if (BeamParticleSystem)
		{
			UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticleSystem->Template,
				TargetPoint,
				FRotator::ZeroRotator,
				true);

			CurrentEmitter->InitializeSystem();
			CurrentEmitter->SetBeamSourcePoint(0, TargetPoint, 0);

			FVector NewPoint = FVector(TargetPoint);
			NewPoint.X = NewPoint.X + BeamLength;  // Beam length in X direction 
			CurrentEmitter->SetBeamTargetPoint(0, NewPoint, 0);
			CurrentEmitter->ActivateSystem();
		}
	}

}

