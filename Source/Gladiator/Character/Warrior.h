// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Warrior.generated.h"

UCLASS()
class GLADIATOR_API AWarrior : public ACharacter
{
	GENERATED_BODY()

	protected:
	/*	Components	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UActorSocketComponent* WeaponSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UActorSocketComponent* ShieldSocket;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FName ShieldSocketName;

	/*	Properties	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warrior")
	float MaxLife;

	UPROPERTY(BlueprintReadOnly, Category = "Warrior")
	float Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warrior")
	float MovementSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Warrior")
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category = "Warrior")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipement")
	class AWeapon* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipement")
	class AShield* Shield;

	/*	Functions	*/

	virtual void BeginPlay() override;
	virtual void Reset() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	
	UFUNCTION()
	virtual void OnWeaponHit(AWarrior* OtherWarrior, int Damage);

	UFUNCTION(BlueprintCallable, Category = "Warrior")
	virtual void DropEquipement(class AEquipement* Equipement, class UActorSocketComponent* ActorSocket);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Warrior")
	void SetLife(float Value);

	UFUNCTION(BlueprintNativeEvent, Category = "Warrior")
	void OnHit(int OtherDamage);
	UFUNCTION(BlueprintNativeEvent, Category = "Warrior")
	void OnDeath();

	public:
	AWarrior();

	UFUNCTION(BlueprintCallable, Category = "Warrior")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "Warrior")
	virtual bool PickEquipement(class AEquipement* Equipement);

	virtual float GetLife() const;
	virtual float GetMaxLife() const;
	virtual bool IsDead() const;

	UFUNCTION(BlueprintCallable, Category = "Warrior")
	virtual bool CanAttack() const;

	virtual AWeapon* GetWeapon() const;
	virtual AShield* GetShield() const;

	virtual bool IsAttacking() const;
	virtual void OnNotifyAttack(bool bAttacking);
};
