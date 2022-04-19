// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRPlayerCharacter.generated.h"

UCLASS()
class VR_SWORDFIGHT_API AVRPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int RotationAmount = 45.0;//The increments of the rotation snapping

	UPROPERTY(EditAnywhere, meta = (UIMin = 0.0, UIMax = 1.0))
	float RotationThreshold = 0.3;//Threshold at which rotation snapping is activated
	
	UPROPERTY(EditAnywhere)
	FVector HandOffset; //The offset of the right skeletal mesh on the right side

	UPROPERTY(EditAnywhere)
	float HandScale; //The scale of the right skeletal mesh on the right side

	UPROPERTY(EditAnywhere)
	FVector HandRotation; //The rotation of the right skeletal mesh on the right side

	// Sets default values for this character's properties
	AVRPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* VRParent;

	UPROPERTY(VisibleAnywhere)
	class UPostProcessComponent* PostProcessComponent;

	UPROPERTY(VisibleAnywhere)
	class UVRHandComponent* HandLeft;

	UPROPERTY(VisibleAnywhere) 
	class UVRHandComponent* HandRight;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMesh* HandSkeletalMesh;

	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* BlinkerMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UCurveFloat* BlinkerCurve;

	UMaterialInstanceDynamic* DynamicBlinkerMaterial;

	FVector LastCameraLocation;
	FRotator OriginalRotation;

	bool bCanRotate = true;

	UFUNCTION()
	void MoveForward(float AxisValue); 

	UFUNCTION()
	void MoveRight(float AxisValue);

	UFUNCTION()
	void Rotate(float AxisValue);//Rotates VRParent in the desired direction

	UFUNCTION()
	void SnapToOriginalRotation();//Rotates VRParent back to its original orientation

	void AdjustColliderLocation();//Moves the collider to the camera position and moves VRParent in the opposite direction
	
	void SetupBlinkerMaterial();//Sets up the blinker material as a post process effect

	void UpdateBlinker(); //Updates the blinker according to the players speed
};
