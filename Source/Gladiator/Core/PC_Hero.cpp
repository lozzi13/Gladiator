// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_Hero.h"

#include "Gladiator.h"
#include "GladiatorGameModeBase.h"
#include "AI/AIDirector.h"

#include "Character/Hero.h"
#include "Character/Enemy.h"

#include "Props/Equipement.h"

#include "GameFramework/CharacterMovementComponent.h"

/*	Setup	*/

APC_Hero::APC_Hero()
{
	MouseSensibility.X = 100.f;
	MouseSensibility.Y = 100.f;

	bIsLocking = false;
	EnemyLockedIndex = 0;
	CrtEnemyLocked = nullptr;
}

void APC_Hero::SetupInputComponent()
{
	if (!InputComponent)
	{
		InputComponent = NewObject<UInputComponent>(this, TEXT("PC_InputComponent0"));
		InputComponent->RegisterComponent();
	}

	InputComponent->BindAxis("Vertical", this, &APC_Hero::MoveVertical);
	InputComponent->BindAxis("Horizontal", this, &APC_Hero::MoveHorizontal);
	InputComponent->BindAxis("MouseVertical", this, &APC_Hero::MouseVertical);
	InputComponent->BindAxis("MouseHorizontal", this, &APC_Hero::MouseHorizontal);
	InputComponent->BindAxis("NextEnemyLocked", this, &APC_Hero::UpdateEnemyLocked);

	InputComponent->BindAction("Attack", IE_Pressed, this, &APC_Hero::LeftClick);
	InputComponent->BindAction("Quit", IE_Released, this, &APC_Hero::Quit);
	InputComponent->BindAction("Lock", IE_Pressed, this, &APC_Hero::ToggleLock);
}

void APC_Hero::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!bIsLocking || !IsValid(CrtEnemyLocked) || !IsValid(Hero))
		return;

	FRotator ControlRot { GetControlRotation() };
	ControlRot.Pitch = 0.0f;

	SetControlRotation(ControlRot);
	Hero->LerpRotation(ControlRot, DeltaTime * RotationSpeed);
}

void APC_Hero::Reset()
{
	Super::Reset();

	bIsLocking = false;
	EnemyLockedIndex = 0;
	CrtEnemyLocked = nullptr;
}

bool APC_Hero::IsLocking()
{
	return bIsLocking;
}

void APC_Hero::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	Hero = Cast<AHero>(GetPawn());
	if (!Hero)
		UnPossess();
}

/*	Inputs	*/

void APC_Hero::MoveVertical(float Value)
{
	if (!IsValid(Hero) || Hero->IsAttacking() || Hero->IsDead())
		return;

	FRotator ControlRot { GetControlRotation() };
	ControlRot.Pitch = 0.0f;

	FVector MoveDir { ControlRot.RotateVector(FVector::ForwardVector) };

	UCharacterMovementComponent* Movement { Hero->GetCharacterMovement() };
	if (Movement)
		Movement->AddInputVector(MoveDir * Value);
}

void APC_Hero::MoveHorizontal(float Value)
{
	if (!IsValid(Hero) || Hero->IsAttacking() || Hero->IsDead())
		return;

	FRotator ControlRot { GetControlRotation() };
	ControlRot.Pitch = 0.0f;

	FVector MoveDir = ControlRot.RotateVector(FVector::RightVector);

	UCharacterMovementComponent* Movement { Hero->GetCharacterMovement() };
	if (Movement)
		Movement->AddInputVector(MoveDir * Value);
}

void APC_Hero::MouseVertical(float Value)
{
	float Input = Value * MouseSensibility.Y;
	
	if (!bIsLocking)
		AddPitchInput(Input);
}

void APC_Hero::MouseHorizontal(float Value)
{
	float Input = Value * MouseSensibility.X;

	if (!bIsLocking)
		AddYawInput(Input);
}

void APC_Hero::LeftClick()
{
	if (!IsValid(Hero))
		return;

	if (Hero->CanAttack())
	{
		Hero->Attack();
		return;
	}
	
	TSet<AActor*> OverlappedActors {};
	Hero->GetOverlappingActors(OverlappedActors, AEquipement::StaticClass());

	if (OverlappedActors.Num() <= 0)
		return;

	TSet<AActor*>::TIterator It { OverlappedActors.CreateIterator() };
	for (; It != end(OverlappedActors) ; ++It)
	{
		if (!IsValid(*It))
			continue;

		bool bPicked { Hero->PickEquipement(Cast<AEquipement>(*It)) };
		if (bPicked)
			return;
	}
}

void APC_Hero::Quit()
{
	#ifdef WITH_EDITOR
		UKismetSystemLibrary::QuitGame(GetWorld(), this, EQuitPreference::Quit);
	#else
		FGenericPlatformMisc::RequestExit(false);
	#endif
}

/*	Lock	*/

void APC_Hero::ToggleLock()
{
	bIsLocking = !bIsLocking;

	if (bIsLocking)
		OnEnemyUpdate();

	else if (IsValid(CrtEnemyLocked))
		CrtEnemyLocked->SetLocked(false);

	else
		return;

	if (IsValid(Hero))
		Hero->SetPawnRotation(bIsLocking);
}

void APC_Hero::UpdateEnemyLocked(float Value)
{
	if (!bIsLocking)
		return;

	EnemyLockedIndex +=  Value;
	OnEnemyUpdate();
}

void APC_Hero::OnEnemyUpdate()
{
	AGameModeBase* Mode{ UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode{ Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;

	UAIDirector* Dir = GameMode->GetAIDirector();
	if (!Dir)
		return;

	CrtEnemyLocked = Cast<AEnemy>(Dir->GetEnemy(EnemyLockedIndex));
	if (!IsValid(CrtEnemyLocked))
		return;
}

void APC_Hero::RepossessCamera(FRotator Rotation)
{
	SetControlRotation(Rotation);
}

FRotator APC_Hero::GetControlRotation() const
{
	FRotator ControlRot {};
	if (bIsLocking && IsValid(CrtEnemyLocked) && IsValid(Hero))
	{
		ControlRot = UKismetMathLibrary::FindLookAtRotation(Hero->GetActorLocation(),
															CrtEnemyLocked->GetActorLocation());
	}
	else
		ControlRot = Super::GetControlRotation();

	return ControlRot;
}