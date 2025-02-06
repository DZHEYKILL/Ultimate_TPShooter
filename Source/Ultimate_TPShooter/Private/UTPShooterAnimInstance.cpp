// Fill out your copyright notice in the Description page of Project Settings.


#include "UTPShooterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UUTPShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (ShooterCharacter == nullptr)
	{
		ShooterCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	}
	if (ShooterCharacter)
	{
		//Speed
		FVector Velocity = ShooterCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		//Air Check
		bIsInAir = ShooterCharacter->GetCharacterMovement()->IsFalling();

		//Acceleration bool
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else 
		{
			bIsAccelerating = false;
		}
	}
}

void UUTPShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}
