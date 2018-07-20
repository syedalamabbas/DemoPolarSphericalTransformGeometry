// Fill out your copyright notice in the Description page of Project Settings.

#include "DynConcentricSpheresGrid.h"
#include "CppBeamEmitter.h"



// Sets default values
ADynConcentricSpheresGrid::ADynConcentricSpheresGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnRoot"));
	RootComponent = WhereToSpawn;


	WhereToSpawnTransformed = CreateDefaultSubobject<UBoxComponent>(TEXT("WhereToSpawnTransformed"));
	WhereToSpawnTransformed->SetupAttachment(RootComponent);
	WhereToSpawnTransformed->SetRelativeLocation(FVector(-200.0f, 0.0f, -70.0f)); // Just a simple displaced Box

	BeamParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DefaultEmptyParticleSystem"));
	BeamParticleSystem->SetupAttachment(RootComponent);

	// Loading some grid defaults
	K = 10;
	M = 10;
	N = 9;
	GridSpacing = 15;
	
}

// Called when the game starts or when spawned
void ADynConcentricSpheresGrid::BeginPlay()
{
	Super::BeginPlay();
	ConstructGrids();
}

// Called every frame
void ADynConcentricSpheresGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

//	FString deltaString = FString::SanitizeFloat(DeltaTime);
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deltaString);

	if (bForwardTransform)
	{
		TransformSphericalToPlanar(DeltaTime);
	}

	if (bReverseTransform)
	{
		TransformPlanarToSpherical(DeltaTime);
	}
}

void ADynConcentricSpheresGrid::TransformSphericalToPlanar(float DeltaTime)
{
	
	for (int32 Index = 0; Index != CurrentActorInLocation.Num(); ++Index)
	{
		AActor* currentActor = CurrentActorInLocation[Index];
		FVector DesiredLocation = DesiredNewPositions[Index];
		float InterpSpeed = 0.5f;
		FVector destinationLocation = FMath::VInterpTo(currentActor->GetActorLocation(), DesiredLocation, DeltaTime, InterpSpeed);
		currentActor->SetActorLocation(destinationLocation, true);
	}
}

void ADynConcentricSpheresGrid::TransformPlanarToSpherical(float DeltaTime)
{
	for (int32 Index = 0; Index != CurrentActorInLocation.Num(); ++Index)
	{
		AActor* currentActor = CurrentActorInLocation[Index];
		FVector DesiredLocation = CurrentPositions[Index];
		float InterpSpeed = 0.5f;
		FVector destinationLocation = FMath::VInterpTo(currentActor->GetActorLocation(), DesiredLocation, DeltaTime, InterpSpeed);
		currentActor->SetActorLocation(destinationLocation, true);
	}
}

void ADynConcentricSpheresGrid::SetDynamicallyColorOfActors(AActor* currentActor, int32 ActorIndex)
{
	TArray<FColor> myColors;

	// HardCoding for now
	myColors.Add(FColor::White); // White 
	myColors.Add(FColor::Red); // Red 
	myColors.Add(FColor::Green); // Green
	myColors.Add(FColor::Blue); // Blue

	UStaticMeshComponent* m_MeshComp = (UStaticMeshComponent*)currentActor->GetRootComponent();
	// Mobility
	m_MeshComp->SetMobility(EComponentMobility::Movable);
	UStaticMesh* m_Mesh = m_MeshComp->GetStaticMesh();
	UMaterialInterface* m_MaterialofMesh = m_Mesh->GetMaterial(0);
	UMaterialInstanceDynamic * DynamicMaterial = UMaterialInstanceDynamic::Create(m_MaterialofMesh, NULL);
	int32 currentColorIndex = FMath::Abs(ActorIndex) - 1;
	if (currentColorIndex >= 0)
	{
		DynamicMaterial->SetVectorParameterValue(FName("Color"), FLinearColor(myColors[currentColorIndex]));
		DynamicMaterial->SetScalarParameterValue(FName("Metallic"), 100.0f);
		DynamicMaterial->SetScalarParameterValue(FName("Roughness"), 0.0f);
		m_MeshComp->SetMaterial(0, DynamicMaterial);
	}
}

void ADynConcentricSpheresGrid::ConstructGrids()
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
						AActor* currentActor = GetWorld()->SpawnActor<AActor>(WhatToSpawn, TargetMeshLocation, FRotator::ZeroRotator, SpawnParams);
						SetDynamicallyColorOfActors(currentActor, n);
						
						if (n != 0)
						{
							FVector NewDesiredLocationForAnimation;
							FVector NewOrigin = WhereToSpawnTransformed->GetComponentLocation();
							NewDesiredLocationForAnimation.X = NewOrigin.X + k*GridSpacing;
							NewDesiredLocationForAnimation.Y = NewOrigin.Y + m*GridSpacing;
							NewDesiredLocationForAnimation.Z = NewOrigin.Z + 2* FMath::Abs(n)*GridSpacing;
							CurrentActorInLocation.Add(currentActor);
							DesiredNewPositions.Add(NewDesiredLocationForAnimation);
							CurrentPositions.Add(TargetMeshLocation);
						}

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