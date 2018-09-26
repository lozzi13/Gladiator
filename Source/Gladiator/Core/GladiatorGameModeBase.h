// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameModeBase.h"
#include "GladiatorGameModeBase.generated.h"

UCLASS()
class GLADIATOR_API AGladiatorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	protected:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "In Game")
	int EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "In Game", meta = (AllowPrivateAccess = "true"))
	class UAIDirector* AIDirector;

	bool bEnded;

	virtual void Reset() override;

	UFUNCTION(BlueprintNativeEvent)
	void Win();

	UFUNCTION(BlueprintNativeEvent)
	void Lose();

	public:
	AGladiatorGameModeBase();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameStateChanged);

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FGameStateChanged OnWin;

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FGameStateChanged OnLose;

	UFUNCTION(BlueprintCallable)
	virtual void Restart();

	UFUNCTION(BlueprintCallable)
	virtual void Quit();

	virtual void NotifyEnemyCount(int CountToAdd);

	virtual int GetNbEnemy();
	virtual class UAIDirector* GetAIDirector();

	virtual void NotifyEnemySpawned(class AWarrior* Enemy);
	virtual void NotifyEnemyKilled(class AWarrior* Enemy);
	virtual void NotifyPlayerKilled();

	virtual void ResetLevel() override;

	UFUNCTION(BlueprintCallable)
	virtual void CanAttack();
};
