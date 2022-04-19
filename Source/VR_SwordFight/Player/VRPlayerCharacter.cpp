// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerCharacter.h"

#include "VRHandComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PostProcessComponent.h"

// Sets default values
AVRPlayerCharacter::AVRPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRParent = CreateDefaultSubobject<USceneComponent>(TEXT("VRParent"));
	VRParent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(VRParent);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AVRPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetupBlinkerMaterial();
	if (VRParent) {
		OriginalRotation = VRParent->GetRelativeRotation();
	}
	if (CameraComponent) {
		LastCameraLocation = CameraComponent->GetRelativeLocation();
	}

	HandLeft = UVRHandComponent::MAKE(this, VRParent, EControllerHand::Left, HandSkeletalMesh, HandOffset, HandRotation, HandScale);
	HandRight = UVRHandComponent::MAKE(this, VRParent, EControllerHand::Right, HandSkeletalMesh, HandOffset, HandRotation, HandScale);
}

// Called every frame
void AVRPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AdjustColliderLocation();
	UpdateBlinker();
}

// Called to bind functionality to input
void AVRPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("SnapToOriginalRotation"), EInputEvent::IE_Pressed, this, &AVRPlayerCharacter::SnapToOriginalRotation);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AVRPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AVRPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Rotate"), this, &AVRPlayerCharacter::Rotate);
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

void AVRPlayerCharacter::Rotate(float AxisValue) {
	float Sign = FMath::Sign(AxisValue);
	float AbsoluteValue = FMath::Abs(AxisValue);
	if (AbsoluteValue > RotationThreshold) {
		if (VRParent) {
			if (bCanRotate) {
				VRParent->AddLocalRotation(FQuat(GetActorUpVector(), FMath::DegreesToRadians(Sign * RotationAmount)));
				bCanRotate = false;
			}
		}
	}
	else {
		bCanRotate = true;
	}
}

void AVRPlayerCharacter::SnapToOriginalRotation() {
	if (VRParent) {
		VRParent->SetRelativeRotation(OriginalRotation);
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

void AVRPlayerCharacter::SetupBlinkerMaterial() {
	DynamicBlinkerMaterial = UMaterialInstanceDynamic::Create(BlinkerMaterial, this);
	if (PostProcessComponent) {
		PostProcessComponent->AddOrUpdateBlendable(DynamicBlinkerMaterial);
		//DynamicBlinkerMaterial->SetScalarParameterValue(TEXT("Radius"), 0);
	}
}

void AVRPlayerCharacter::UpdateBlinker() {
	if (DynamicBlinkerMaterial && BlinkerCurve) {
		float Speed = GetVelocity().Length();
		float Radius = BlinkerCurve->GetFloatValue(Speed);
		DynamicBlinkerMaterial->SetScalarParameterValue(TEXT("Radius"), Radius);
	}
}