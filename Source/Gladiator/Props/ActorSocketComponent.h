// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "ActorSocketComponent.generated.h"

UCLASS(ClassGroup = (Custom), BlueprintType, meta = (BlueprintSpawnableComponent))
class GLADIATOR_API UActorSocketComponent : public USceneComponent
{
	GENERATED_BODY()

	protected:
	/** Class to instanciate when calling SpawnChild */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Socket")
	TSubclassOf<AActor> DefaultChildActorClass;

	/** Class that this ActorSocket accepts in SetChildActor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Socket")
	TSubclassOf<AActor> ChildActorClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Socket")
	AActor* ChildActor;

	/** If bOwnsChild is true, the Child will be deleted with the component. */
	bool bOwnsChild;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnAttachmentChanged() override;

	public:
	UActorSocketComponent();

	UFUNCTION(BlueprintCallable, Category = "Actor Socket")
	virtual bool SpawnChild(bool bIsOwner = true);

	UFUNCTION(BlueprintCallable, Category = "Actor Socket")
	virtual bool SetChildActor(AActor* NewChild, bool bBecomeOwner = true);

	UFUNCTION(BlueprintCallable, Category = "Actor Socket")
	virtual AActor* RemoveChild();

	virtual AActor* GetChildActor() const;

	virtual void SetChildActorClass(TSubclassOf<AActor> NewClass);
	virtual TSubclassOf<AActor> GetChildActorClass() const;

	virtual void SetDefaultChildActorClass(TSubclassOf<AActor> NewClass);
	virtual TSubclassOf<AActor> GetDefaultChildActorClass() const;
};
