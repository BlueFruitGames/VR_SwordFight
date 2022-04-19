// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "VRHandComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VR_SWORDFIGHT_API UVRHandComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UVRHandComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Generates a new UVRHandComponent and also sets up it subcomponents
	static UVRHandComponent* MAKE(AActor* ParentActor, USceneComponent* ParentComponent, EControllerHand TrackingSource, USkeletalMesh* HandSkelatalMesh, FVector HandOffset, FVector HandRotation, float HandScale);

	void SetHandMesh(class USkeletalMeshComponent* HandMesh_In) {
		HandMesh = HandMesh_In;
	};

	void SetMotionControllerComponent(class UMotionControllerComponent* MotionControllerComponent_In) {
		MotionControllerComponent = MotionControllerComponent_In;
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class USkeletalMeshComponent* HandMesh;	

	UPROPERTY()
	class UMotionControllerComponent* MotionControllerComponent;

};
