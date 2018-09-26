// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rain.generated.h"

#define RAIN_MINIMUM_INTENSITY 0.5f

UCLASS()
class GLADIATOR_API ARain : public AActor
{
	GENERATED_BODY()

	public:
	ARain();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRainChanged, bool, bIsRaining, float, Intensity, float, Duration);

	UPROPERTY(BlueprintAssignable, Category = "State")
	FRainChanged OnRain;

	protected:
	/*	Components	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UExponentialHeightFogComponent* Fog;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UTimelineComponent* RainTimeline;

	/*	Parameters	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain")
	class UCurveFloat* FogDensityCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain")
	class UCurveFloat* RainIntensityCurve;

	/** Number of particles to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain", meta = (ClampMin = 0.f))
	float MaxParticles;

	/** Delay to wait between rains */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain", meta = (ClampMin = 0.f))
	float DelayMin;

	/** Delay to wait between rains */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain", meta = (ClampMin = 0.f))
	float DelayMax;

	/** How long does it rain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain", meta = (ClampMin = 0.f))
	float DurationMin;

	/** How long does it rain */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rain", meta = (ClampMin = 0.f))
	float DurationMax;

	/*	State	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rain State")
	bool bIsRaining;
	bool bCanRain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rain State")
	float CrtIntensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rain State")
	float CrtDuration;

	FTimerHandle RainWaitHandle;
	FTimerHandle RainStopHandle;

	/*	Functions	*/

	virtual void BeginPlay() override;
	virtual void Reset() override;

	virtual void WaitForRain();
	virtual void StartRain();
	virtual void StopRain();

	UFUNCTION()
	virtual void OnGameEnded();

	UFUNCTION()
	virtual void FogDensityUpdate(float Density);

	UFUNCTION()
	virtual void RainIntensityUpdate(float Intensity);
};
