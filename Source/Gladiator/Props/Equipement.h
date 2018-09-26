// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Equipement.generated.h"

UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType, Abstract, meta = (BlueprintSpawnableComponent))
class GLADIATOR_API AEquipement : public AActor
{
	GENERATED_BODY()

	protected:
	/*	Components	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (EditCondition = true))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (EditCondition = true))
	class USphereComponent* BoundingVolume;

	/*	Properties	*/

	/** Probabiblity between 0 and 1 that the equipement is dropped. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipement", meta = (ClampMin=0.f, ClampMax=1.f))
	float DropProbability;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipement")
	class AWarrior* Holder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipement")
	FVector ForceWhenDropped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipement")
	float DelayBeforePicked;

	FTimerHandle PickTimerHandle;

	bool bDestroyOnReset;
	bool bCanBePicked;

	/*	Functions	*/

	virtual void BeginPlay() override;
	virtual void Reset() override;

	void SetCanBePicked();

	UFUNCTION(BlueprintNativeEvent)
	void OnDropped();

	UFUNCTION(BlueprintNativeEvent)
	void OnPicked();

	public:
	AEquipement();

	UFUNCTION(BlueprintCallable)
	virtual bool IsHeld();

	UFUNCTION(BlueprintCallable)
	virtual bool CanBePicked() const;
	virtual float GetDropProbability() const;

	virtual void Drop(class UActorSocketComponent* ActorSocket);
	virtual bool Pick(class AWarrior* NewHolder, class UActorSocketComponent* ActorSocket);
};
