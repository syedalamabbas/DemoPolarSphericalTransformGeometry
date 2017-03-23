// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SphericalPolarGrid.generated.h"

UCLASS()
class CPPBEAMEMITTER_API ASphericalPolarGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphericalPolarGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Where to Spawn is the location of origin of the grid
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WhereToSpawn;            // This is the anchor

	// What to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> WhatToSpawn;

	// Beam particle to be used for the grid lines
	UPROPERTY(EditAnywhere, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* BeamParticleSystem;



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

	void ConstructGrid();

};
