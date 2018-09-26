// Fill out your copyright notice in the Description page of Project Settings.

#include "DayCycleLight.h"

#include "Gladiator.h"

#include "Runtime/Engine/Classes/Components/SkyLightComponent.h"
#include "Runtime/Engine/Classes/Components/DirectionalLightComponent.h"
#include "Runtime/Engine/Classes/Engine/PostProcessVolume.h"

ADayCycleLight::ADayCycleLight()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	DirectionalLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DirectionalLight"));
	DirectionalLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	DirectionalLight->SetRelativeRotation(BaseRotation);

	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void ADayCycleLight::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(SkySphere))
	{
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	DirectionalLight->Intensity = bIsNight ? 0.f : BaseIntensity;
	DirectionalLight->MarkRenderStateDirty();
	SkyLight->Intensity = bIsNight ? 0.f : SkyIntensity;
}

void ADayCycleLight::Reset()
{
	GetWorldTimerManager().ClearTimer(ShiftHandle);
	bIsNight = false;

	DirectionalLight->SetRelativeRotation(BaseRotation);
	DirectionalLight->Intensity = bIsNight ? 0.f : BaseIntensity;
	DirectionalLight->MarkRenderStateDirty();
	SkyLight->Intensity = bIsNight ? 0.f : SkyIntensity;
}

void ADayCycleLight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SkyLight->RecaptureSky();

	float Speed { bIsNight ? NightSpeed : DaySpeed };
	FRotator Rotation { DeltaTime * Speed, 0.f, 0.f };

	DirectionalLight->AddLocalRotation(Rotation);
	Rotation = DirectionalLight->GetComponentTransform().Rotator();

	bool bShouldBeNight { Rotation.Pitch >= NightThreshold && Rotation.Pitch <= 100.f };
	if (bShouldBeNight == bIsNight)
		return;

	StartShift(bShouldBeNight);
}

void ADayCycleLight::StartShift(bool bShouldBeNight)
{
	bIsNight = bShouldBeNight;

	FTimerManager& Time { GetWorldTimerManager() };
	Time.ClearTimer(ShiftHandle);

	if (bIsNight)
		Time.SetTimer(ShiftHandle, this, &ADayCycleLight::FadeToNight, FadeUpdateDelay, true);

	else
		Time.SetTimer(ShiftHandle, this, &ADayCycleLight::FadeToDay, FadeUpdateDelay, true);

	OnTimeChanged.Broadcast(bShouldBeNight);
}

void ADayCycleLight::FadeToDay()
{
	if (!IsValid(DirectionalLight) || DirectionalLight->Intensity >= BaseIntensity)
	{
		GetWorldTimerManager().ClearTimer(ShiftHandle);
		return;
	}

	DirectionalLight->Intensity += IntensitySpeed;

	if (DirectionalLight->Intensity > BaseIntensity)
		DirectionalLight->Intensity = BaseIntensity;

	DirectionalLight->MarkRenderStateDirty();
}

void ADayCycleLight::FadeToNight()
{
	if (!IsValid(DirectionalLight) || DirectionalLight->Intensity <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(ShiftHandle);
		return;
	}

	DirectionalLight->Intensity -= IntensitySpeed;

	if (DirectionalLight->Intensity < 0.f)
		DirectionalLight->Intensity = 0.f;

	DirectionalLight->MarkRenderStateDirty();
}

void ADayCycleLight::UpdateAmbient(const FLinearColor& Tint, float Intensity)
{
	if (!IsValid(PostProcessVolume))
		return;
	
	PostProcessVolume->Settings.AmbientCubemapTint = Tint;
	PostProcessVolume->Settings.AmbientCubemapIntensity = Intensity;
}