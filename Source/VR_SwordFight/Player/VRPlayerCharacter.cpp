// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVRPlayerCharacter::AVRPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRParent = CreateDefaultSubobject<USceneComponent>(TEXT("VRParent"));
	VRParent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(VRParent);
}

// Called when the game starts or when spawned
void AVRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CameraComponent) {
		LastCameraLocation = CameraComponent->GetRelativeLocation();
	}
}

// Called every frame
void AVRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AdjustColliderLocation();
}

// Called to bind functionality to input
void AVRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRPlayerCharacter::MoveRight);
}

void AVRPlayerCharacter::MoveForward(float AxisValue)
{
	if (CameraComponent) {
		AddMovementInput(CameraComponent->GetForwardVector(), AxisValue);
	}
}

void AVRPlayerCharacter::MoveRight(float AxisValue)
{
	if (CameraComponent) {
		AddMovementInput(CameraComponent->GetRightVector(), AxisValue);
	}
}

void AVRPlayerCharacter::AdjustColliderLocation() {
	FVector CurrentCameraLocation = CameraComponent->GetRelativeLocation();
	FVector CameraOffset = CurrentCameraLocation - LastCameraLocation;
	CameraOffset.Z = 0;

	GetCapsuleComponent()->AddWorldOffset(CameraOffset);
	if (VRParent) {
		VRParent->AddWorldOffset(-CameraOffset);
	}
	LastCameraLocation = CurrentCameraLocation;
}