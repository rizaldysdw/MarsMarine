// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Marine.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class MARSMARINE_API AMarine : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMarine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the character receive vertical input
	void MoveForward(float Value);

	// Called when the character receive horizontal input
	void MoveRight(float Value);

	/* Rotate controller based on Mouse X input
	@param Value - The input value from mouse movement */
	void Turn(float Value);

	/* Rotate controller based on Mouse Y input
	@param Value - The input value from mouse movement */
	void LookUp(float Value);

	/* Called via input to turn at a given rate.
	@param Rate - This is a normalized rate, i.e. 1.0 means 100% of desired turn rate */
	void TurnRate(float Rate);

	/* Called via input to look up/down at a given rate.
	@param Rate - This is a normalized rate, i.e. 1.0 means 100 % of desired rate */
	void LookUpRate(float Rate);

	// Should be called when Fire action input is pressed
	void StartFiring();

	// Should be called when Fire action input is released
	void StopFiring();

	// Should be called when Aim action input is pressed
	void StartAiming();

	// Should be called when Aim action input is released
	void StopAiming();

	// Interpolate current camera FOV to target camera FOV
	void CameraAimInterp(float DeltaTime);

	// Equip weapon
	void EquipWeapon(class AWeapon* WeaponToEquip);

	// Spawn default weapon
	class AWeapon* SpawnDefaultWeapon();

	// Perform line trace from crosshair 
	bool CrosshairLineTrace(FHitResult& OutHitResult, FVector& OutHitLocation);

	// True if crosshair line trace hit something. Second line trace is performed here
	bool GetCrosshairLineTraceEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);

	// Function to fire the weapon
	void FireWeapon();

	// Play Fire Anim Montage
	void PlayFireMontage();

	// Play Weapon Shot Sound
	void PlayWeaponShotSound();

	// Send bullet in direction of line trace
	void SendBullet();

	// Should be called in FireWeapon()
	void StartFireTimer();

	// Should be called only if weapon is automatic
	void AutomaticFireReset();

private:
	// Camera Boom positioning behind character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Camera that follows the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CurrentTurnRate;

	// Look up/down rate, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CurrentLookUpRate;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	// Turn rate when aiming, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimTurnRate;

	// Look up/down rate when aiming, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float AimLookUpRate;

	// Mouse turn rate, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseTurnRate;

	// Mouse look up/down rate, in deg/sec. Other scaling may affect final look up rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseLookUpRate;

	// Mouse turn rate when aiming, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimTurnRate;

	// Mouse look up/down rate when aiming, in deg/sec. Other scaling may affect final turn rate
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true", ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float MouseAimLookUpRate;

	// Default camera field of view
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV;

	// Aim camera field of view
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraAimFOV;

	// Current camera field of view value
	float CameraCurrentFOV;

	// Camera interpolation speed when aiming
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	float CameraAimInterpSpeed;

	// Default Weapon class. Set in Blueprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeapon;

	// Currently equipped weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	// Character can only fire or reload when unoccupied
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	// True when Fire input is pressed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bFirePressed;

	// True when Aim input is pressed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool bAimPressed;

	// Montage containing animations for firing weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* FireMontage;

	// Montage containing animations for reloading weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	FTimerHandle AutoFireTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAimPressed() const { return bAimPressed; }
};
