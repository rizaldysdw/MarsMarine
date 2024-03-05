// Fill out your copyright notice in the Description page of Project Settings.


#include "MarineAnimInstance.h"
#include "Marine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UMarineAnimInstance::UMarineAnimInstance() :
	Speed(0.f),
	bIsInAir(false),
	bIsMoving(false),
	MovementOffsetYaw(0.f),
	LastMovementOffsetYaw(0.f),
	Pitch(0.f),
	bAiming(false)
{
}

void UMarineAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	// Cast AnimBP to Marine class
	if (Marine == nullptr)
	{
		Marine = Cast<AMarine>(TryGetPawnOwner());
	}

	// If Marine reference is valid
	if (Marine)
	{
		// Get the lateral speed of the character from velocity
		FVector Velocity{ Marine->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		// Get character aim pitch
		Pitch = Marine->GetBaseAimRotation().Pitch;

		// Check if the character is in air
		bIsInAir = Marine->GetCharacterMovement()->IsFalling();

		// Check if the character is moving
		if (Marine->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
		{
			bIsMoving = true;
		}
		else
		{
			bIsMoving = false;
		}

		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(Marine->GetVelocity());
		FRotator AimRotation = Marine->GetBaseAimRotation();

		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

		if (Marine->GetVelocity().Size() > 0)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}

		bAiming = Marine->GetAimPressed();
	}
}

void UMarineAnimInstance::NativeInitializeAnimation()
{
}
