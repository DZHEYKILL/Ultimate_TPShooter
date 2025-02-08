// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UTPShooterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ULTIMATE_TPSHOOTER_API UUTPShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UFUNCTION(BluePrintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	virtual void NativeInitializeAnimation() override;
	
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class ABaseCharacter* ShooterCharacter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAccelerating;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MovementOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAimingAnim;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bFireAnim;
};
