// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Warrior.h"
#include "Hero.generated.h"

UCLASS()
class GLADIATOR_API AHero : public AWarrior
{
	GENERATED_BODY()

	protected:
	virtual void UnPossessed() override;
	virtual void OnDeath_Implementation() override;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent * Camera;
	
	public:
	AHero();

	void SetPawnRotation(bool bIsLocking);
	void LerpRotation(FRotator Rotation, float Speed);
};
