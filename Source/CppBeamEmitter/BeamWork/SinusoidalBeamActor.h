// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SinusoidalBeamActor.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ESinusoidType : uint8
{
	SINE,
	COSINE
};

//If you want this to appear in BP, make sure to use this instead
USTRUCT(BlueprintType)
//USTRUCT()
struct FSinusoidalParams
{
	GENERATED_USTRUCT_BODY()

		// So to simplify your life for later debugging, always use UPROPERTY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinusoidalParams")
		ESinusoidType WaveType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinusoidalParams")
		float Amplitude; // Tha Amplitude

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinusoidalParams")
		float Frequency; // Tha Frequency

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinusoidalParams", meta = (ClampMin = "0.0", ClampMax = "6.2831853071795864", UIMin = "0.0", UIMax = "6.2831853071795864"))
		float Phase; // Tha Phase - range from 0 - 2*PI

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SinusoidalParams")
		float TimeDelta; // Tha Time Delay or Delta

						 //Get WaveType
	ESinusoidType GetWaveType()
	{
		return WaveType;
	}


	//Get
	float GetAmplitude()
	{
		return Amplitude;
	}

	//Get
	float GetFrequency()
	{
		return Frequency;
	}

	//Get
	float GetPhase()
	{
		return Phase;
	}

	//Get
	float GetTimeDelta()
	{
		return TimeDelta;
	}

	//Set
	void SetWaveType(const ESinusoidType NewValueWave)
	{
		WaveType = NewValueWave;
	}


	//Set
	void SetAmplitude(const float NewValueAmplitude)
	{
		Amplitude = NewValueAmplitude;
	}

	//Set
	void SetFrequency(const float NewValueFrequency)
	{
		Frequency = NewValueFrequency;
	}

	//Set
	void SetPhase(const float NewValuePhase)
	{
		Phase = NewValuePhase;
	}

	//Set
	void SetTimeDelta(const float NewValueTimeDelta)
	{
		TimeDelta = NewValueTimeDelta;
	}

	//Constructor
	FSinusoidalParams()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		WaveType = ESinusoidType::SINE;
		Amplitude = 0;
		Frequency = 0;
		Phase = 0;
		TimeDelta = 0;
	}
};

UCLASS()
class CPPBEAMEMITTER_API ASinusoidalBeamActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASinusoidalBeamActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		UParticleSystemComponent* BeamParticleSystem;

	/*UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ADummyActor> WhatToSpawn;*/

	class UBoxComponent* GetWhereToSpawn();

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetOriginInVolume();


private: 
	void CreateDesiredBeamPattern();


protected:
	// Where to Spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* WhereToSpawn;            // This is the anchor

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TArray<FVector> SpawnLocations;			// These are the actual locations

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FSinusoidalParams SinusoidalParams;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 NumberOfAtoms;
};
