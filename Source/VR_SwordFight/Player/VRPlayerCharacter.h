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
	UPROPERTY()
	class UCameraComponent* CameraComponent;

	UPROPERTY()
	class USceneComponent* VRParent;

	FVector LastCameraLocation;

	UFUNCTION()
	void MoveForward(float AxisValue);

	UFUNCTION()
	void MoveRight(float AxisValue);

	void AdjustColliderLocation();
};
