// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CubeGrid.generated.h"

UCLASS()
class CPPBEAMEMITTER_API ACubeGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeGrid();

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

	// N x N x N are the dimensions of the grid
	UPROPERTY(EditAnywhere, Category = "Spawning") // Uniform radial spacing unit measure
		int N;

	// Uniform spacing unit measure
	UPROPERTY(EditAnyWhere, Category = "Spawning")
		int GridSpacing; 


	void ConstructGrid();


	
};
