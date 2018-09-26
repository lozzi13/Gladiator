// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DayCycleLight.generated.h"

UCLASS()
class GLADIATOR_API ADayCycleLight : public AActor
{
	GENERATED_BODY()

	public:
	ADayCycleLight();

	virtual void Tick(float DeltaTime) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeChanged, bool, bShouldBeNight);

	UPROPERTY(BlueprintAssignable, Category = "Day Cycle")
	FTimeChanged OnTimeChanged;

	protected:
	/*	Component properties	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky & Sun")
	class UDirectionalLightComponent* DirectionalLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky & Sun")
	FRotator BaseRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky & Sun")
	float BaseIntensity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky & Sun")
	class USkyLightComponent* SkyLight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky & Sun")
	float SkyIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky & Sun")
	AActor* SkySphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky & Sun")
	class APostProcessVolume* PostProcessVolume;

	/* Day Cycle properties */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Day Cycle")
	float NightThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Day Cycle")
	float DaySpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Day Cycle")
	float NightSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Day Cycle")
	float IntensitySpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Day Cycle")
	float FadeUpdateDelay;

	UPROPERTY(BlueprintReadOnly, Category = "Day Cycle")
	bool bIsNight;

	FTimerHandle ShiftHandle;

	/*	Functions	*/

	virtual void BeginPlay() override;
	virtual void Reset() override;

	virtual void StartShift(bool bShouldBeNight);

	virtual void FadeToDay();
	virtual void FadeToNight();

	UFUNCTION(BlueprintCallable)
	virtual void UpdateAmbient(const FLinearColor& Tint, float Intensity);
};