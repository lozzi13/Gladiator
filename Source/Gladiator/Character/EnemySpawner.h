// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class GLADIATOR_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

	protected:
	/*	Properties	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Spawner")
	class UClass* EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
	int BaseEnemyCount;

	UPROPERTY(BlueprintReadOnly, Category = "Enemy Spawner")
	int EnemyCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawner")
	float SpawnDelay;

	float CurrentTime;

	/*	Functions*/

	virtual void BeginPlay() override;
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Spawner")
	virtual void Spawn();

	public:
	AEnemySpawner();

	virtual void Tick(float DeltaTime) override;
};
