// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BeamEmitterBaseActor.generated.h"

UCLASS()
class CPPBEAMEMITTER_API ABeamEmitterBaseActor : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	ABeamEmitterBaseActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		UParticleSystemComponent* BeamParticleSystem;

	/*UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<ADummyActor> WhatToSpawn;*/

	FORCEINLINE UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetOriginInVolume();


private: 
	void CreateDesiredBeamPattern();


protected:
	// Where to Spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* WhereToSpawn;            // This is the anchor

	//UPROPERTY(EditAnywhere, Category = "Spawning")
		TArray<FVector> SpawnLocationsSource;			// These are the actual locations
		
};
