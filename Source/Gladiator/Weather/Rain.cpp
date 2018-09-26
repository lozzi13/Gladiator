// Fill out your copyright notice in the Description page of Project Settings.

#include "Rain.h"

#include "Gladiator.h"
#include "Core/GladiatorGameModeBase.h"

#include "Particles/ParticleSystemComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/TimelineComponent.h"

ARain::ARain()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsRaining = false;
	bCanRain = false;
	CrtIntensity = 0.f;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
	ParticleSystem->bAutoActivate = bIsRaining;

	Fog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("Fog"));
	Fog->SetupAttachment(RootComponent);
	Fog->FogDensity = CrtIntensity;

	RainTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("RainTimeline"));

	RainTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	RainTimeline->SetNewTime(0.f);
}

void ARain::BeginPlay()
{
	Super::BeginPlay();

	ParticleSystem->SetFloatParameter(TEXT("SpawnRate"), 0.f);
	bCanRain = true;

	FOnTimelineFloat Callback {};
	Callback.BindUFunction(this, TEXT("FogDensityUpdate"));
	RainTimeline->AddInterpFloat(FogDensityCurve, Callback, TEXT("FogDensity"));

	Callback.BindUFunction(this, TEXT("RainIntensityUpdate"));
	RainTimeline->AddInterpFloat(RainIntensityCurve, Callback, TEXT("RainIntensity"));

	AGameModeBase* ModeBase { UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* InGame { Cast<AGladiatorGameModeBase>(ModeBase) };
	if (IsValid(InGame))
	{
		InGame->OnWin.AddDynamic(this, &ARain::OnGameEnded);
		InGame->OnLose.AddDynamic(this, &ARain::OnGameEnded);
	}

	WaitForRain();
}

void ARain::Reset()
{
	Super::Reset();

	ParticleSystem->SetFloatParameter(TEXT("SpawnRate"), 0.f);
	bCanRain = true;

	WaitForRain();
}

void ARain::WaitForRain()
{
	if (!bCanRain)
		return;

	float RandomDelay { FMath::RandRange(DelayMin, DelayMax) };
	CrtDuration = FMath::RandRange(DurationMin, DurationMax);
	CrtIntensity = FMath::RandRange(RAIN_MINIMUM_INTENSITY, 1.f);

	FTimerManager& Time { GetWorldTimerManager() };
	Time.SetTimer(RainWaitHandle, this, &ARain::StartRain, RandomDelay, false);
}

void ARain::StartRain()
{
	if (bIsRaining)
	{
		WaitForRain();
		return;
	}

	bIsRaining = true;
	ParticleSystem->Activate();

	RainTimeline->Play();
	OnRain.Broadcast(bIsRaining, CrtIntensity, CrtDuration);

	FTimerManager& Time { GetWorldTimerManager() };
	Time.SetTimer(RainStopHandle, this, &ARain::StopRain, CrtDuration, false);
}

void ARain::StopRain()
{
	if (bIsRaining)
	{
		bIsRaining = false;

		RainTimeline->Reverse();
		OnRain.Broadcast(bIsRaining, CrtIntensity, CrtDuration);
	}

	if (bCanRain)
		WaitForRain();
}

void ARain::OnGameEnded()
{
	bCanRain = false;

	FTimerManager& Time { GetWorldTimerManager() };
	Time.ClearTimer(RainWaitHandle);

	if (bIsRaining)
		StopRain();
}

void ARain::FogDensityUpdate(float Density)
{
	Fog->FogDensity = Density * CrtIntensity;
	Fog->MarkRenderStateDirty();
}

void ARain::RainIntensityUpdate(float Intensity)
{
	float NbParticles { Intensity * CrtIntensity * MaxParticles };

	ParticleSystem->SetFloatParameter(TEXT("SpawnRate"), FMath::Clamp(NbParticles, 0.f, MaxParticles));

	if (NbParticles <= 0.f && !bIsRaining)
		ParticleSystem->Deactivate();
}
