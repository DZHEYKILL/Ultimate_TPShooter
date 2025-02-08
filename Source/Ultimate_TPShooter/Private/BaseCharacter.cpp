// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"
#include "DrawDebugHelpers.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Variables
	bAiming = false;
	DefaultFOV = 100.f;
	ZoomFOV = 50.f;
	
	ShootTimerDuration = 0.05f;
	bFiringBullet = false;
	AutomaticFireRate = 0.1f;
	bShouldFire = true;
	bFire = false;

	//Components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->FieldOfView = DefaultFOV;

	WalkSpeed = 500.0f;
	SprintSpeed = 1000.0f;
	CharacterMovement = GetCharacterMovement();
	CharacterMovement->bOrientRotationToMovement = true;
	CharacterMovement->MaxWalkSpeed = WalkSpeed;
	CharacterMovement->MaxAcceleration = 800.0f;
	CharacterMovement->BrakingDecelerationWalking = 800.0f;
	CharacterMovement->BrakingFriction = 0.4f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

//Base Movement
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::fJump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprint);

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &ABaseCharacter::StartFireWeapon);
	PlayerInputComponent->BindAction("FireButton", IE_Released, this, &ABaseCharacter::StopFireWeapon);

	PlayerInputComponent->BindAction("Aiming", IE_Pressed, this, &ABaseCharacter::AimingPressed);
	PlayerInputComponent->BindAction("Aiming", IE_Released, this, &ABaseCharacter::AimingReleased);


}

//Movement
void ABaseCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{

		/*
	CameraForward.Z = 0.0f;
	CameraForward.Normalize();	
		AddMovementInput(CameraForward, Value);
		*/

		FRotator ControlRotation = Controller->GetControlRotation();
		FVector ForwardVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::X); // Получаем ось X (вперёд)

		ForwardVector.Z = 0.0f;
		ForwardVector.Normalize();

		AddMovementInput(ForwardVector, Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		FRotator ControlRotation = Controller->GetControlRotation();
		FVector RightVector = FRotationMatrix(ControlRotation).GetUnitAxis(EAxis::Y); 

		RightVector.Z = 0.0f;
		RightVector.Normalize();
		AddMovementInput(RightVector, Value);
	}
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::fJump()
{
	Jump();
}

void ABaseCharacter::StartSprint()
{
	CharacterMovement->MaxWalkSpeed = SprintSpeed;
}

void ABaseCharacter::StopSprint()
{
	CharacterMovement->MaxWalkSpeed = WalkSpeed;

}

//Combat
void ABaseCharacter::FireWeapon()
{
	

	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}

	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("BarrelSocket");
	FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());

	if (BarrelSocket)
	{
		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}
		/*
		FHitResult HitResult;
		FVector TraceStart = SocketTransform.GetLocation();
		FQuat Rotation = SocketTransform.GetRotation();
		FVector RotationAxis = Rotation.GetAxisX();
		FVector TraceEnd = TraceStart + RotationAxis * 50'000.0f;

		GetWorld()->LineTraceSingleByChannel
			(HitResult,
			TraceStart,
			TraceEnd,
			ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f);
			DrawDebugPoint(GetWorld(), HitResult.Location, 5.f, FColor::Green, false, 2.0f);
			if (ImpactPerticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactPerticle, HitResult.Location);
			}
		}
		*/
	}
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}
	FVector2D CrosshairLocation(ViewPortSize.X / 2.0f, ViewPortSize.Y / 2.0f);
	CrosshairLocation.Y -= 40.0f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, 
		CrosshairWorldPosition, 
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		FHitResult ScreenTraceHit;
		FVector TraceStart = CrosshairWorldPosition;
		FVector TraceEnd = CrosshairWorldPosition + CrosshairWorldDirection * 50'000.f;

		FVector BeamEndPoint = TraceEnd;
		GetWorld()->LineTraceSingleByChannel(ScreenTraceHit,
			TraceStart,
			TraceEnd,
			ECC_Visibility);
		if (ScreenTraceHit.bBlockingHit)
		{
			BeamEndPoint = ScreenTraceHit.Location;
			if (ImpactPerticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactPerticle, ScreenTraceHit.Location);
			}
		}
	}
	UAnimInstance* AnimInstanse = GetMesh()->GetAnimInstance();
	if (AnimInstanse && HipFireMontage)
	{
		AnimInstanse->Montage_Play(HipFireMontage);
		AnimInstanse->Montage_JumpToSection(FName("StartFire"));
	}
	StartCrosshairBulletFire();

	
}

void ABaseCharacter::StartFireWeapon()
{
	bFire = true;
	StartFireTimer();
}

void ABaseCharacter::StopFireWeapon()
{
	bFire = false;
}

void ABaseCharacter::StartFireTimer()
{
	if (bShouldFire) {

		FireWeapon();
		bShouldFire = false;
		GetWorldTimerManager().SetTimer(AutoFireTimer, this, &ABaseCharacter::AutoFireRest, AutomaticFireRate);

	}
}

void ABaseCharacter::AutoFireRest()
{
	bShouldFire = true;
	if (bFire)
	{
		StartFireTimer();
	}
}

void ABaseCharacter::AimingPressed()
{
	bAiming = true;
	Camera->FieldOfView = ZoomFOV;
	bUseControllerRotationYaw = bAiming;

}

void ABaseCharacter::AimingReleased()
{
	bAiming = false;
	Camera->FieldOfView = DefaultFOV;
	bUseControllerRotationYaw = bAiming;

}

void ABaseCharacter::StartCrosshairBulletFire()
{
	bFiringBullet = true;

	GetWorldTimerManager().SetTimer(CrosshairShootTimer, 
		this, 
		&ABaseCharacter::FinishCrosshairBulletFire,
		ShootTimerDuration);
}

void ABaseCharacter::FinishCrosshairBulletFire()
{
	bFiringBullet = false;
}

