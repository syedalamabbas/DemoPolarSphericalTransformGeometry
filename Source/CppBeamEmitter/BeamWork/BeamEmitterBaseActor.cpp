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
	BeamParticleSystem->InitializeSystem();
	BeamParticleSystem->ActivateSystem();
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
	int32 NumberOfBeams = 10;
	//SpawnLocationsSource.SetNum(NumberOfBeams, false);
	int32 BeamLength = 200;
	int32 ZOffset = 20;

	BeamParticleSystem->Activate();

	FVector BoxLocation = GetOriginInVolume();
	FName SocketName = NAME_None;

	FVector TargetPoint;


	for (int32 j = 0; j < NumberOfBeams+1; j++)
	{ 
		for (int32 i = 0; i < NumberOfBeams+2; i++)  // Along X Axis
		{
			TargetPoint.X = BoxLocation.X;                 // Constant
			TargetPoint.Y = BoxLocation.Y + j*ZOffset;                 // Constant
			TargetPoint.Z = BoxLocation.Z + i*ZOffset;   // Moving in Straight Line

			if (BeamParticleSystem)
			{
				UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAttached(
					BeamParticleSystem->Template, //UParticleSystem*
					RootComponent,
					SocketName,
					TargetPoint, //relative offset 
					FRotator(0,0, 0), //relative rotation
					EAttachLocation::KeepWorldPosition,
					true //auto delete on completion
				);
			}
		}
	}

	for (int32 j = 0; j < NumberOfBeams+1; j++)
	{
		for (int32 i = 0; i < NumberOfBeams+2; i++)  // Along X Axis
		{
			TargetPoint.X = BoxLocation.X + i*ZOffset;                 // Constant
			TargetPoint.Y = BoxLocation.Y + j*ZOffset;                 // Constant
			TargetPoint.Z = BoxLocation.Z ;   // Moving in Straight Line

			if (BeamParticleSystem)
			{
				UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAttached(
					BeamParticleSystem->Template, //UParticleSystem*
					RootComponent,
					SocketName,
					TargetPoint, //relative offset 
					FRotator(90, 90, 0), //relative rotation
					EAttachLocation::KeepWorldPosition,
					true //auto delete on completion
				);
			}
		}
	}
	

	for (int32 j = 0; j < NumberOfBeams+1; j++)
	{
		for (int32 i = 0; i < NumberOfBeams+2; i++)  // Along X Axis
		{
			TargetPoint.X = BoxLocation.X + i*ZOffset;                 // Constant
			TargetPoint.Y = BoxLocation.Y ;                 // Constant
			TargetPoint.Z = BoxLocation.Z + j*ZOffset;   // Moving in Straight Line
			if (BeamParticleSystem)
			{
				UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAttached(
					BeamParticleSystem->Template, //UParticleSystem*
					RootComponent,
					SocketName,
					TargetPoint, //relative offset 
					FRotator(0, 90, 90), //relative rotation
					EAttachLocation::KeepWorldPosition,
					true //auto delete on completion
				);
			}
		}
	}
	
	
}

