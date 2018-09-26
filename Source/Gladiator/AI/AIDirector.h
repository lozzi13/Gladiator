// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "AIDirector.generated.h"

UCLASS()
class GLADIATOR_API UAIDirector : public UActorComponent
{
	GENERATED_BODY()

	protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int NbEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalAngleRot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDelay;
	FTimerHandle TimeHandle;

	TArray<class AWarrior*> AICharacters;
	class AWarrior* CurrentAttacker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHostile;

	virtual void BeginPlay() override;

	virtual void RecalculateAngles();
	virtual void OrderAttack();

	public:
	UAIDirector();

	virtual void AddEnemy(class AWarrior* Actor);
	virtual void RemoveEnemy(class AWarrior* Actor);
	virtual class AWarrior* GetEnemy(int Index);

	virtual void Reset();

	virtual void LockEnemy(int Index);
	virtual void UnlockAll();
	virtual void SetIsHostile(bool bHostile);
};