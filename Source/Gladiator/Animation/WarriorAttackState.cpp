// Fill out your copyright notice in the Description page of Project Settings.

#include "WarriorAttackState.h"

#include "WarriorAnim.h"

void UWarriorAttackState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	UAnimInstance* AnimInstance { MeshComp->GetAnimInstance() };
	UWarriorAnim* WarriorAnim { Cast<UWarriorAnim>(AnimInstance) };
	if (!IsValid(WarriorAnim))
		return;

	WarriorAnim->NotifyAttack(true);
}

void UWarriorAttackState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UAnimInstance* AnimInstance { MeshComp->GetAnimInstance() };
	UWarriorAnim* WarriorAnim { Cast<UWarriorAnim>(AnimInstance) };
	if (!IsValid(WarriorAnim))
		return;

	WarriorAnim->NotifyAttack(false);
}

FString UWarriorAttackState::GetNotifyName_Implementation() const
{
	return "Warrior Attack";
}
