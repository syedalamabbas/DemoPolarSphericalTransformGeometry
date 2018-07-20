// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DynConcentricSpheresGrid.generated.h"

UCLASS()
class CPPBEAMEMITTER_API ADynConcentricSpheresGrid : public AActor
{
	GENERATED_BODY()
	
private:
	TArray<AActor*> CurrentActorInLocation;  // This is spherical arrangement of Mesh Actors

	TArray<FVector> CurrentPositions; //  spherical arrangement

	TArray<FVector> DesiredNewPositions; // This is planar arrangement of spherical data at different radii

	void SetDynamicallyColorOfActors(AActor* currentActor, int32 ActorIndex);
		
public:	
	// Sets default values for this actor's properties
	ADynConcentricSpheresGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Where to Spawn is the location of origin of the grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawn;            // This is the anchor


	// Where to Spawn is the location of origin of the grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* WhereToSpawnTransformed;            // This is the anchor for new arrangement


	// What to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> WhatToSpawn;

	// Beam particle to be used for the grid lines
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* BeamParticleSystem;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// K x M x N are the dimensions of the grid
	UPROPERTY(EditAnywhere, Category = "Spawning") // Uniform radial spacing unit measure
		int N;

	// Zenith
	UPROPERTY(EditAnywhere, Category = "Spawning") // Uniform Azimuth Angular Measure
		int K;

	// Azimuth
	UPROPERTY(EditAnywhere, Category = "Spawning") // Uniform Elevation Angular Measure
		int M;

	// Unit uniform measure 
	UPROPERTY(EditAnywhere, Category = "Spawning") // Uniform Elevation Angular Measure
		int GridSpacing;

	void ConstructGrids();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning") // Forward Transform
	bool bForwardTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning") // Forward Transform
	bool bReverseTransform;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void TransformSphericalToPlanar(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void TransformPlanarToSpherical(float DeltaTime);
	
};
