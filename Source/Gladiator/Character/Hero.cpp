// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero.h"

#include "Gladiator.h"
#include "Core/PC_Hero.h"
#include "Core/GladiatorGameModeBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;

	UCapsuleComponent* Capsule { GetCapsuleComponent() };
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_PLAYER);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void AHero::OnDeath_Implementation()
{
	APC_Hero* Controller = Cast<APC_Hero>(GetController());
	if (Controller->IsLocking())
	{
		Controller->ToggleLock();

		if (SpringArm)
			Controller->RepossessCamera(SpringArm->GetComponentRotation());
	}

	Super::OnDeath_Implementation();
	AGameModeBase* Mode { UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode { Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;

	GameMode->NotifyPlayerKilled();
}

void AHero::UnPossessed()
{
	APC_Hero* Controller = Cast<APC_Hero>(GetController());
	if (Controller->IsLocking())
	{
		Controller->ToggleLock();

		if (SpringArm)
			Controller->RepossessCamera(SpringArm->GetComponentRotation());
	}
	
	UCharacterMovementComponent* Movement { GetCharacterMovement() };
	if (IsValid(Movement))
		Movement->StopMovementImmediately();

	Super::UnPossessed();
}

void AHero::SetPawnRotation(bool bIsLocking)
{
	if (!SpringArm)
		return;

	SpringArm->bUsePawnControlRotation = !bIsLocking;
}

void AHero::LerpRotation(FRotator Rotation, float Speed)
{
	FRotator DstRotation { FMath::Lerp(GetActorRotation(), Rotation, Speed) };
	SetActorRotation(DstRotation);

	if (SpringArm)
		SpringArm->SetWorldRotation(Rotation);
}