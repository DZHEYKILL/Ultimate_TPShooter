// Fill out your copyright notice in the Description page of Project Settings.


#include "UTPShooterAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

		bAimingAnim = ShooterCharacter->bAiming;

		bFireAnim = ShooterCharacter->bFire;
		//Acceleration bool
		if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else 
		{
			bIsAccelerating = false;
		}
		FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();

		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
		MovementOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
		
		
		FString OffsetMessage = FString::Printf(TEXT("Movement Offset Yaw: %f"), MovementOffset);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::White, OffsetMessage);
		}
	}
}

void UUTPShooterAnimInstance::NativeInitializeAnimation()
{
	ShooterCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}
