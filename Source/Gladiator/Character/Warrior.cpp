// Fill out your copyright notice in the Description page of Project Settings.

#include "Warrior.h"

#include "Gladiator.h"
#include "Props/Weapon.h"
#include "Props/Shield.h"
#include "Props/ActorSocketComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

AWarrior::AWarrior()
{
	PrimaryActorTick.bCanEverTick = true;

	Life = MaxLife = 1.f;
	MovementSpeed = 600.f;

	// Set Capsule Collider
	UCapsuleComponent* Capsule { GetCapsuleComponent() };
	Capsule->InitCapsuleSize(63.f, 45.f);

	Capsule->bGenerateOverlapEvents = true;
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_PICKABLE, ECollisionResponse::ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY, ECollisionResponse::ECR_Block);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_PLAYER, ECollisionResponse::ECR_Block);

	// Create Equipement socket
	USkeletalMeshComponent* Mesh { GetMesh() };

	WeaponSocket = CreateDefaultSubobject<UActorSocketComponent>(TEXT("WeaponSocket"));
	WeaponSocket->SetupAttachment(Mesh);
	WeaponSocket->bAutoActivate = false;

	ShieldSocket = CreateDefaultSubobject<UActorSocketComponent>(TEXT("ShieldSocket"));
	ShieldSocket->SetupAttachment(Mesh);
	WeaponSocket->bAutoActivate = false;
}

void AWarrior::BeginPlay()
{
	Super::BeginPlay();

	// Set properties
	Life = MaxLife;
	bIsAttacking = false;
	bIsDead = false;

	// Spawn equipement
	USkeletalMeshComponent* Mesh { GetMesh() };

	WeaponSocket->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	WeaponSocket->SpawnChild(false);

	ShieldSocket->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, ShieldSocketName);
	ShieldSocket->SpawnChild(false);

	// Get equipement
	Weapon = Cast<AWeapon>(WeaponSocket->GetChildActor());
	if (IsValid(Weapon))
		Weapon->OnHit().BindUObject(this, &AWarrior::OnWeaponHit);

	Shield = Cast<AShield>(ShieldSocket->GetChildActor());

	// Set speed
	UCharacterMovementComponent* Movement { GetCharacterMovement() };
	if (Movement)
		Movement->MaxWalkSpeed = MovementSpeed;
}

void AWarrior::Reset()
{
	Super::Reset();

	// Reset properties
	Life = MaxLife;
	bIsAttacking = false;
	bIsDead = false;

	// Respawn/Get equipement
	WeaponSocket->SpawnChild(false);
	Weapon = Cast<AWeapon>(WeaponSocket->GetChildActor());

	if (IsValid(Weapon))
		Weapon->OnHit().BindUObject(this, &AWarrior::OnWeaponHit);
	
	ShieldSocket->SpawnChild(false);
	Shield = Cast<AShield>(ShieldSocket->GetChildActor());
}

void AWarrior::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);

	if (IsValid(Weapon))
		Weapon->Destroy();

	if (IsValid(Shield))
		Shield->Destroy();
}

void AWarrior::OnWeaponHit(AWarrior* OtherWarrior, int Damage)
{
	if (!IsValid(Weapon))
		return;

	OtherWarrior->OnHit(Damage);

	// Calculate if drop weapon
	float Rand { FMath::RandRange(0.f, 1.f) };
	if (Rand <= Weapon->GetDropProbability())
		DropEquipement(Weapon, WeaponSocket);
}

void AWarrior::Attack()
{
	if (!CanAttack())
		return;

	bIsAttacking = true;
}

bool AWarrior::PickEquipement(AEquipement* Equipement)
{
	// Pick weapon
	AWeapon* NewWeapon { Cast<AWeapon>(Equipement) };
	if (IsValid(NewWeapon) && !IsValid(Weapon))
	{
		if (!NewWeapon->Pick(this, WeaponSocket))
			return false;

		Weapon = NewWeapon;			
		Weapon->OnHit().BindUObject(this, &AWarrior::OnWeaponHit);
		return true;
	}
	
	// Pick shield
	AShield* NewShield { Cast<AShield>(Equipement) };
	if (!IsValid(NewShield) || IsValid(Shield))
		return false;

	if (!NewShield->Pick(this, ShieldSocket))
		return false;

	Shield = NewShield;
	return true;
}

void AWarrior::DropEquipement(AEquipement* Equipement, UActorSocketComponent* ActorSocket)
{
	if (!Equipement)
		return;

	Equipement->Drop(ActorSocket);

	if (Equipement == Weapon)
		Weapon = nullptr;
	else
		Shield = nullptr;
}

float AWarrior::GetLife() const
{
	return Life;
}

float AWarrior::GetMaxLife() const
{
	return MaxLife;
}

bool AWarrior::IsAttacking() const
{
	return bIsAttacking;
}

bool AWarrior::IsDead() const
{
	return bIsDead;
}

bool AWarrior::CanAttack() const
{
	return IsValid(Weapon) && !bIsDead && !bIsAttacking;
}

void AWarrior::OnNotifyAttack(bool bAttacking)
{
	if (!IsValid(Weapon))
	{
		bIsAttacking = false;
		return;
	}

	Weapon->SetCollision(bAttacking);

	if (!bAttacking)
		bIsAttacking = false;
}

void AWarrior::SetLife_Implementation(float Value)
{
	Life = Value;

	if (Life <= 0.f)
		OnDeath();
}

void AWarrior::OnHit_Implementation(int OtherDamage)
{
	if (Life <= 0.f)
		return;

	float Damage { (float)OtherDamage };
	if (!IsValid(Shield))
		Damage *= 2.f;

	// Calculate if drop shield
	else
	{
		float Rand { FMath::RandRange(0.f, 1.f) };
		if (Rand <= Shield->GetDropProbability())
			DropEquipement(Shield, ShieldSocket);
	}

	SetLife(Life - Damage);
}

void AWarrior::OnDeath_Implementation()
{
	bIsDead = (Life <= 0.f);
}

AWeapon* AWarrior::GetWeapon() const
{
	return Weapon;
}

AShield* AWarrior::GetShield() const
{
	return Shield;
}