// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "WarriorAnim.generated.h"

UCLASS()
class GLADIATOR_API UWarriorAnim : public UAnimInstance
{
	GENERATED_BODY()

	protected:
	/*	Properties	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class AWarrior* Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D Speed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHitting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bCanStrafe;

	public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	virtual void NotifyAttack(bool bAttacking);
};
