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
	int32 NumberOfBeams = 100;
	int32 BeamLength = 500;
	int32 ZOffset = 5;

	FVector BoxLocation = GetOriginInVolume();
	FVector BeamSourcePoint;

	UParticleSystem* TemplateParticleSystem = BeamParticleSystem->Template;
	
	for (int32 i = 0; i <= NumberOfBeams; i++)  // Along X Axis
	{
		BeamSourcePoint.X = BoxLocation.X;                 // Constant
		BeamSourcePoint.Y = BoxLocation.Y;                 // Constant
		BeamSourcePoint.Z = BoxLocation.Z + i*ZOffset;     // Moving in Straight Line

		if (BeamParticleSystem)
		{
			FVector BeamTargetPoint = FVector(BeamSourcePoint);
			BeamTargetPoint.X = BeamTargetPoint.X + BeamLength;  // Beam length in X direction 

			// Prepare On Fly altering the source and target points -- This thing is doing nothing
			//CurrentTemp->SetBeamSourcePoint(0, BeamSourcePoint, 0);
			//CurrentTemp->SetBeamTargetPoint(0, BeamTargetPoint, 0);
			//CurrentTemp->SetBeamEndPoint(i, BeamTargetPoint);

			// Spawning
			UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticleSystem->Template,
				BeamSourcePoint,
				FRotator::ZeroRotator,
				true);

			CurrentEmitter->SetBeamSourcePoint(0, BeamSourcePoint, 0);
			CurrentEmitter->SetBeamTargetPoint(0, BeamTargetPoint, 0);
		}
	}

	for (int32 i = 0; i <= NumberOfBeams; i++)  // Along X Axis
	{
		BeamSourcePoint.X = BoxLocation.X + i*ZOffset;                 // Constant
		BeamSourcePoint.Y = BoxLocation.Y;                 // Constant
		BeamSourcePoint.Z = BoxLocation.Z ;     // Moving in Straight Line

		if (BeamParticleSystem)
		{
			FVector BeamTargetPoint = FVector(BeamSourcePoint);
			BeamTargetPoint.Z = BeamTargetPoint.Z + BeamLength;  // Beam length in X direction 

			// Spawning
			UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
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

