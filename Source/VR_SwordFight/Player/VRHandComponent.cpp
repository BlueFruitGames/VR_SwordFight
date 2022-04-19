// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHandComponent.h"

#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values for this component's properties
UVRHandComponent::UVRHandComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

UVRHandComponent* UVRHandComponent::MAKE(AActor* ParentActor, USceneComponent* ParentComponent, EControllerHand TrackingSource, USkeletalMesh* HandSkelatalMesh, FVector HandOffset, FVector HandRotation, float HandScale)
{
	UVRHandComponent* HandComponent;
	USkeletalMeshComponent* SkeletalMeshComponent;
	UMotionControllerComponent* MotionControllerComponent;
	if (TrackingSource == EControllerHand::Left) {
		HandComponent = NewObject<UVRHandComponent>(ParentActor, UVRHandComponent::StaticClass(), TEXT("LeftHand"));
		MotionControllerComponent = NewObject<UMotionControllerComponent>(ParentActor, UMotionControllerComponent::StaticClass(), TEXT("LeftMotionController"));
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(ParentActor, USkeletalMeshComponent::StaticClass(), TEXT("LeftHandMesh"));
	}
	else {
		HandComponent = NewObject<UVRHandComponent>(ParentActor, UVRHandComponent::StaticClass(), TEXT("RightHand"));
		MotionControllerComponent = NewObject<UMotionControllerComponent>(ParentActor, UMotionControllerComponent::StaticClass(), TEXT("RightMotionController"));
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(ParentActor, USkeletalMeshComponent::StaticClass(), TEXT("RightHandMesh"));
	}
	
	HandComponent->AttachToComponent(ParentComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	MotionControllerComponent->SetTrackingSource(TrackingSource);
	MotionControllerComponent->AttachToComponent(HandComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	SkeletalMeshComponent->SetSkeletalMesh(HandSkelatalMesh);
	SkeletalMeshComponent->AttachToComponent(MotionControllerComponent, FAttachmentTransformRules::KeepRelativeTransform);

	HandComponent->SetMotionControllerComponent(MotionControllerComponent);
	HandComponent->SetHandMesh(SkeletalMeshComponent);
	if (TrackingSource == EControllerHand::Left) {
		SkeletalMeshComponent->SetWorldRotation(FRotator(HandRotation.X, HandRotation.Y, -HandRotation.Z).Quaternion());
		SkeletalMeshComponent->SetRelativeScale3D(FVector(HandScale, -HandScale, HandScale));
		HandComponent->SetRelativeLocation(FVector(HandOffset.X, -HandOffset.Y, HandOffset.Z));
	}
	else {
		SkeletalMeshComponent->SetWorldRotation(FRotator(HandRotation.X, HandRotation.Y, HandRotation.Z).Quaternion());
		SkeletalMeshComponent->SetRelativeScale3D(FVector(HandScale));
		HandComponent->SetRelativeLocation(HandOffset);
	}
	
	MotionControllerComponent->RegisterComponent();
	SkeletalMeshComponent->RegisterComponent();
	HandComponent->RegisterComponent();
	return HandComponent;
}



// Called when the game starts
void UVRHandComponent::BeginPlay()
{
	Super::BeginPlay();
		
}


// Called every frame
void UVRHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

