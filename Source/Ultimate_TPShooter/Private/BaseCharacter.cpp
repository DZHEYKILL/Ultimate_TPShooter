// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//Base Components
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	bUseControllerRotationYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

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

	PlayerInputComponent->BindAction("FireButton", IE_Pressed, this, &ABaseCharacter::FireWeapon);


}
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

void ABaseCharacter::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this,FireSound);
	}
}

