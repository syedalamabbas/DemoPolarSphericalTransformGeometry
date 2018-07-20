// Fill out your copyright notice in the Description page of Project Settings.

#include "SinusoidalBeamActor.h"
#include "CppBeamEmitter.h"

// Sets default values
ASinusoidalBeamActor::ASinusoidalBeamActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnRoot"));
	RootComponent = WhereToSpawn;

	BeamParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DefaultEmptyParticleSystem"));
	BeamParticleSystem->SetupAttachment(RootComponent);

	// Initial values 
	NumberOfAtoms = 100;
	SinusoidalParams.SetAmplitude(50.f);
	SinusoidalParams.SetFrequency(0.001f);
	SinusoidalParams.SetPhase(0.0f);
	SinusoidalParams.SetWaveType(ESinusoidType::SINE);
	SinusoidalParams.SetTimeDelta(3.0f);

}

// Called when the game starts or when spawned
void ASinusoidalBeamActor::BeginPlay()
{
	Super::BeginPlay();
	CreateDesiredBeamPattern();
}

// Called every frame
void ASinusoidalBeamActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}


UBoxComponent* ASinusoidalBeamActor::GetWhereToSpawn()
{
	return WhereToSpawn;
}


FVector ASinusoidalBeamActor::GetOriginInVolume()
{
	return WhereToSpawn->GetComponentLocation();
}

void ASinusoidalBeamActor::CreateDesiredBeamPattern()
{
	SpawnLocations.SetNum(NumberOfAtoms, false);
	FVector BoxLocation = GetOriginInVolume();

	// Calculating locations based on Sinuosidal Patterns 
	for (int32 i = 0; i < NumberOfAtoms; i++)
	{
		SpawnLocations[i].X = BoxLocation.X + i*SinusoidalParams.TimeDelta;   // Moving in Straight Line
		SpawnLocations[i].Y = BoxLocation.Y;                 // Constant
		float ZOutput = 0.0f;
		if (SinusoidalParams.WaveType == ESinusoidType::SINE)
		{
			ZOutput = SinusoidalParams.Amplitude * FMath::Sin(2 * PI* SinusoidalParams.Frequency *i* SinusoidalParams.TimeDelta + SinusoidalParams.Phase);
		}
		if (SinusoidalParams.WaveType == ESinusoidType::COSINE)
		{
			ZOutput = SinusoidalParams.Amplitude * FMath::Cos(2 * PI* SinusoidalParams.Frequency *i* SinusoidalParams.TimeDelta + SinusoidalParams.Phase);
		}
		SpawnLocations[i].Z = BoxLocation.Z + ZOutput;   // Moving in Sinusoidally
	}

	// Spawning Emitters 
	UParticleSystem* TemplateParticleSystem = BeamParticleSystem->Template;
	FVector BeamSourcePoint;
	FVector BeamTargetPoint;
	for (int32 i = 0; i < NumberOfAtoms; i++)
	{
		BeamSourcePoint = SpawnLocations[i];
		if ((i != NumberOfAtoms - 1) && BeamParticleSystem)
		{
			BeamTargetPoint = SpawnLocations[i + 1];
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