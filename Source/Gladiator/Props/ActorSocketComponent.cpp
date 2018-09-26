// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorSocketComponent.h"

#include "Gladiator.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"

UActorSocketComponent::UActorSocketComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bOwnsChild = true;
}

void UActorSocketComponent::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoActivate && !ChildActor)
		SpawnChild(bOwnsChild);
}

void UActorSocketComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	RemoveChild();
}

void UActorSocketComponent::OnAttachmentChanged()
{
	Super::OnAttachmentChanged();

	USceneComponent* Parent { GetAttachParent() };
	if (!IsValid(Parent) || !IsValid(ChildActor))
		return;
	
	ChildActor->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());
}

bool UActorSocketComponent::SpawnChild(bool bIsOwner)
{
	if (IsValid(ChildActor) || !IsValid(DefaultChildActorClass))
		return false;

	UWorld* World {GetWorld()};
	if (!IsValid(World))
		return false;

	USceneComponent* Parent { GetAttachParent() };
	if (!IsValid(Parent))
		return false;

	ChildActor = World->SpawnActor(*DefaultChildActorClass);
	ChildActor->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());

	bOwnsChild = bIsOwner;

	return true;
}

bool UActorSocketComponent::SetChildActor(AActor* NewChild, bool bBecomeOwner)
{
	if (NewChild == ChildActor)
		return true;

	if (IsValid(ChildActor))
		RemoveChild();

	if (!IsValid(NewChild) || !NewChild->IsA(*ChildActorClass))
		return false;

	USceneComponent* Parent { GetAttachParent() };
	if (!IsValid(Parent))
		return false;

	ChildActor = NewChild;
	ChildActor->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GetAttachSocketName());
	bOwnsChild = bBecomeOwner;

	return true;
}

AActor* UActorSocketComponent::RemoveChild()
{
	if (!IsValid(ChildActor))
		return nullptr;

	ChildActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (!bOwnsChild)
	{
		AActor* PreviousChild { ChildActor };
		ChildActor = nullptr;
		return PreviousChild;
	}

	ChildActor->Destroy();
	ChildActor = nullptr;
	return nullptr;
}

AActor* UActorSocketComponent::GetChildActor() const
{
	return ChildActor;
}

void  UActorSocketComponent::SetChildActorClass(TSubclassOf<AActor> NewClass)
{
	ChildActorClass = NewClass;

	if (!IsValid(ChildActor) || ChildActor->IsA(ChildActorClass))
		return;

	RemoveChild();
}

TSubclassOf<AActor> UActorSocketComponent::GetChildActorClass() const
{
	return ChildActorClass;
}

void  UActorSocketComponent::SetDefaultChildActorClass(TSubclassOf<AActor> NewClass)
{
	DefaultChildActorClass = NewClass;
}

TSubclassOf<AActor> UActorSocketComponent::GetDefaultChildActorClass() const
{
	return DefaultChildActorClass;
}