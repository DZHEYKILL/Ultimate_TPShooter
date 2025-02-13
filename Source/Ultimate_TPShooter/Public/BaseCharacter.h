// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USoundCue;
class UParticleSystem;
class UAnimMontage;
class AItem;
class AWeapon;

UCLASS()
class ULTIMATE_TPSHOOTER_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UCharacterMovementComponent* CharacterMovement;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FVector CameraForward;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	FVector CameraRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	AItem* TraceHitItemLastFrame;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	USoundCue* FireSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* MuzzleFlash;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	UParticleSystem* ImpactPerticle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	UAnimMontage* HipFireMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bFire;
	bool bShouldFire;
	float AutomaticFireRate;
	FTimerHandle AutoFireTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bAiming;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float DefaultFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float ZoomFOV;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	float ShootTimerDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bFiringBullet;
	FTimerHandle CrosshairShootTimer;

	bool bShouldTraceForItems;

	int8 OverlappedItemCount;
	//Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	void LookUp(float Value);
	void Turn(float Value);

	void fJump();
	void StartSprint();
	void StopSprint();

	//Combat
	void FireWeapon();
	void StartFireWeapon();
	void StopFireWeapon();

	void StartFireTimer();

	void AutoFireRest();

	void AimingPressed();
	void AimingReleased();
	
	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();
	UFUNCTION()
	bool TraceUnderCrosshairs(FHitResult& OutHitResult,FVector& OutHitLocation);

	FORCEINLINE int8 GetOverlappedItemCount() const {

		return OverlappedItemCount;
	}

	void IncrementOverlappedItemCount(int8 Amount);

	void TraceForItems();

	void SpawnDefaultWeapon();

	void EquipWeapon(AWeapon* WeaponToEquip);

};
