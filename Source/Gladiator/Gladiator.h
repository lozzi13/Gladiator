// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/ConstructorHelpers.h"

#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Math/Rotator.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#define ECC_ENEMY ECC_GameTraceChannel11
#define ECC_PLAYER ECC_GameTraceChannel12
#define ECC_PICKABLE ECC_GameTraceChannel13