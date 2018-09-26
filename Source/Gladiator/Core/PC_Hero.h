// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PC_Hero.generated.h"

UCLASS()
class GLADIATOR_API APC_Hero : public APlayerController
{
	GENERATED_BODY()
	
	protected:
	/*	Properties	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Control")
	FVector2D MouseSensibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Control", meta = (ClampMin = 0.f))
	float RotationSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Control")
	bool bIsLocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Control", meta = (ClampMin = 0))
	int EnemyLockedIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Control")
	class AEnemy* CrtEnemyLocked;
	class AHero* Hero;

	/*	Setup	*/

	virtual void SetupInputComponent() override;
	virtual void Reset() override;

	/*	Inputs	*/

	virtual void MoveHorizontal(float Value);
	virtual void MoveVertical(float Value);

	virtual void MouseHorizontal(float Value);
	virtual void MouseVertical(float Value);
	
	virtual void LeftClick();
	virtual void Quit();

	/*	Lock	*/

	UFUNCTION(BlueprintCallable)
	virtual void OnEnemyUpdate();

	virtual void UpdateEnemyLocked(float value);

	public:
	APC_Hero();

	virtual void PlayerTick(float DeltaTime) override;
	virtual void Possess(APawn* InPawn) override;

	UFUNCTION(BlueprintCallable)
	virtual void ToggleLock();
	virtual bool IsLocking();

	UFUNCTION(BlueprintCallable)
	virtual void RepossessCamera(FRotator Rotation);
	virtual FRotator GetControlRotation() const override;
};
