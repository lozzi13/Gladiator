// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Props/Equipement.h"
#include "Weapon.generated.h"

UCLASS()
class GLADIATOR_API AWeapon : public AEquipement
{
	GENERATED_BODY()

	public:
	AWeapon();

	DECLARE_DELEGATE_TwoParams(FWeaponHitEvent, AWarrior*, int);

	virtual int GetDamage() const;
	virtual FWeaponHitEvent& OnHit();

	virtual void SetCollision(bool bEnabled);
	virtual void Drop(class UActorSocketComponent* ActorSocket) override;

	protected:
	/*	Properties	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (EditCondition = true))
	class UPrimitiveComponent* Collider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equipement")
	int Damage;

	FWeaponHitEvent HitEvent;

	/*	Functions	*/

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void BeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnColliderOverlap(class AWarrior* OtherWarrior, class UPrimitiveComponent* OtherComp);
};
