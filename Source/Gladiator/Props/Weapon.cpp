// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

#include "Character/Warrior.h"
#include "Components/PrimitiveComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	Damage = 1;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(Collider))
	{
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Collider->bGenerateOverlapEvents = true;

		Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		Collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

		Collider->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::BeginOverlap);
	}
}

void AWeapon::OnColliderOverlap(AWarrior* OtherWarrior, UPrimitiveComponent* OtherComp)
{
	HitEvent.ExecuteIfBound(OtherWarrior, Damage);
}

void AWeapon::BeginOverlap(	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
								int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsHeld() || OtherActor == Holder)
		return;

	AWarrior* Warrior { Cast<AWarrior>(OtherActor) };
	if (!Warrior)
		return;

	OnColliderOverlap(Warrior, OtherComp);
}

void AWeapon::SetCollision(bool bEnabled)
{
	if (!IsHeld() || !IsValid(Collider))
		return;

	Collider->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
}

AWeapon::FWeaponHitEvent& AWeapon::OnHit()
{
	return HitEvent;
}

int AWeapon::GetDamage() const
{
	return Damage;
}

void AWeapon::Drop(UActorSocketComponent* ActorSocket)
{
	HitEvent.Unbind();
	if (IsValid(Collider))
		Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Super::Drop(ActorSocket);
}
