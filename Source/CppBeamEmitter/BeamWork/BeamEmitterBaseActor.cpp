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
	int32 NumberOfBeams = 2;
	int32 BeamLength = 200;
	int32 ZOffset = 20;

	FVector BoxLocation = GetOriginInVolume();
	FVector BeamSourcePoint;

	UParticleSystem* TemplateParticleSystem = BeamParticleSystem->Template;
	
	for (int32 i = 0; i < NumberOfBeams; i++)  // Along X Axis
	{
		BeamSourcePoint.X = BoxLocation.X;                 // Constant
		BeamSourcePoint.Y = BoxLocation.Y;                 // Constant
		BeamSourcePoint.Z = BoxLocation.Z + i*ZOffset;   // Moving in Straight Line

		if (BeamParticleSystem)
		{
			// Prepare
			//UParticleSystemComponent* CurrentTemp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TempParticleSystem"));
			//CurrentTemp->SetTemplate(TemplateParticleSystem);
			UParticleSystemComponent* CurrentTemp = BeamParticleSystem;
			
			// On Fly altering the source and target points
			CurrentTemp->SetBeamSourcePoint(0, BeamSourcePoint, 0);
			FVector BeamTargetPoint = FVector(BeamSourcePoint);
			BeamTargetPoint.X = BeamTargetPoint.X + BeamLength;  // Beam length in X direction 
			CurrentTemp->SetBeamTargetPoint(0, BeamTargetPoint, 0);

			// Spawning
			UParticleSystemComponent* CurrentEmitter = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				CurrentTemp->Template,
				BeamSourcePoint,
				FRotator::ZeroRotator,
				true);

			// Destroy
			//CurrentTemp->DestroyComponent();
			
			
		}
	}

}

