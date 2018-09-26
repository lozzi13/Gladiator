// Fill out your copyright notice in the Description page of Project Settings.

#include "Equipement.h"

#include "Gladiator.h"
#include "Character/Warrior.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ActorSocketComponent.h"

AEquipement::AEquipement()
{
	PrimaryActorTick.bCanEverTick = true;
	DropProbability = 0.f;
	bDestroyOnReset = false;

	// Setup Mesh

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->bGenerateOverlapEvents = false;

	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Setup Bounding Volume

	BoundingVolume = CreateDefaultSubobject<USphereComponent>(TEXT("BoundingVolume"));
	BoundingVolume->SetupAttachment(RootComponent);
	BoundingVolume->SetSphereRadius(50.f);

	BoundingVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoundingVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY, ECollisionResponse::ECR_Overlap);
	BoundingVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_PLAYER, ECollisionResponse::ECR_Overlap);
	BoundingVolume->SetCollisionObjectType(ECollisionChannel::ECC_PICKABLE);

	BoundingVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoundingVolume->bGenerateOverlapEvents = true;
}

void AEquipement::BeginPlay()
{
	Super::BeginPlay();

	Holder = Cast<AWarrior>(GetAttachParentActor());
}

void AEquipement::Reset()
{
	Super::Reset();

	if (bDestroyOnReset)
		Destroy();
}

void AEquipement::SetCanBePicked()
{
	bCanBePicked = true;
}

bool AEquipement::CanBePicked() const
{
	return bCanBePicked;
}

float AEquipement::GetDropProbability() const
{
	return DropProbability;
}

void AEquipement::Drop(UActorSocketComponent* ActorSocket)
{
	// Detaching
	if (IsValid(ActorSocket) && ActorSocket->GetChildActor() == this)
	{
		ActorSocket->RemoveChild();
		Holder = nullptr;
	}

	// Start physics
	Mesh->bShouldUpdatePhysicsVolume = true;
	Mesh->SetSimulatePhysics(true);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoundingVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Add starting force
	if (ForceWhenDropped != FVector::ZeroVector)
	{
		FVector Force {};
		Force += GetActorForwardVector() * ForceWhenDropped.X;
		Force += GetActorRightVector() * ForceWhenDropped.Y;
		Force += GetActorUpVector() * ForceWhenDropped.Z;
		Mesh->AddImpulse(Force);
	}

	bDestroyOnReset = true;

	// Can't be picked for a certain amount of time
	bCanBePicked = DelayBeforePicked <= 0.f;
	if (!bCanBePicked)
	{
		FTimerManager& Time { GetWorldTimerManager() };
		Time.SetTimer(PickTimerHandle, this, &AEquipement::SetCanBePicked, DelayBeforePicked, false);
	}

	OnDropped();
}

bool AEquipement::Pick(AWarrior* NewHolder, UActorSocketComponent* ActorSocket)
{
	if (!IsValid(ActorSocket) || IsValid(Holder) || !CanBePicked())
		return false;

	// Stop physics before attaching
	Mesh->bShouldUpdatePhysicsVolume = false;
	Mesh->SetSimulatePhysics(false);

	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoundingVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Attaching
	if (!ActorSocket->SetChildActor(this, false))
		return false;

	Holder = NewHolder;
	bDestroyOnReset = false;

	OnPicked();
	return true;
}

bool AEquipement::IsHeld()
{
	if (IsValid(Holder))
		return true;

	Holder = Cast<AWarrior>(GetAttachParentActor());
	return IsValid(Holder);
}

void AEquipement::OnDropped_Implementation()
{}

void AEquipement::OnPicked_Implementation()
{}