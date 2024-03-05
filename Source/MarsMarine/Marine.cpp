// Fill out your copyright notice in the Description page of Project Settings.


#include "Marine.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AMarine::AMarine() :
	// Camera
	CurrentTurnRate(60.f),
	CurrentLookUpRate(60.f),
	BaseTurnRate(60.f),
	BaseLookUpRate(60.f),
	AimTurnRate(30.f),
	AimLookUpRate(30.f),
	MouseTurnRate(1.f),
	MouseLookUpRate(1.f),
	MouseAimTurnRate(.2f),
	MouseAimLookUpRate(.2f),
	CameraDefaultFOV(0.f), // Set in BeginPlay()
	CameraAimFOV(60.f),
	CameraCurrentFOV(0.f),
	CameraAimInterpSpeed(30.f),

	// Combat
	bFirePressed(false),
	bAimPressed(false)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a Camera Boom
	// Pulls in towards the character if there is a collision
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Camera follow at this distance behind the character
	CameraBoom->TargetArmLength = 300.f;

	// Rotate the arm based on the controller
	CameraBoom->bUsePawnControlRotation = true;

	// Set socket offset of camera boom
	CameraBoom->SocketOffset = FVector(0, 70.0f, 70.0f);

	// Create a Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	// Attach camera to the end of CameraBoom
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Camera does not rotate relative to the arm
	FollowCamera->bUsePawnControlRotation = false;

	// Do not rotate when the controller rotates
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Rotate character based on input direction
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Rotation rate of the character
	GetCharacterMovement()->RotationRate = FRotator(0, 540.0f, 0);

	// Character jump height
	GetCharacterMovement()->JumpZVelocity = 600.0f;

	// How much control the player have when in air
	GetCharacterMovement()->AirControl = 0.2f;
}

// Called when the game starts or when spawned
void AMarine::BeginPlay()
{
	Super::BeginPlay();
	
	if (FollowCamera)
	{
		CameraDefaultFOV = GetFollowCamera()->FieldOfView;
	}

	EquipWeapon(SpawnDefaultWeapon());
}

void AMarine::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0)
	{
		// Find out which way is forward
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };

		AddMovementInput(Direction, Value);
	}
}

void AMarine::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0)
	{
		// Find out which way is right
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };

		AddMovementInput(Direction, Value);
	}
}

void AMarine::Turn(float Value)
{
	float TurnScaleFactor{};

	if (bAimPressed)
	{
		TurnScaleFactor = MouseAimTurnRate;
	}
	else
	{
		TurnScaleFactor = MouseTurnRate;
	}

	AddControllerYawInput(Value * TurnScaleFactor);
}

void AMarine::LookUp(float Value)
{
	float LookUpScaleFactor{};

	if (bAimPressed)
	{
		LookUpScaleFactor = MouseAimLookUpRate;
	}
	else
	{
		LookUpScaleFactor = MouseLookUpRate;
	}

	AddControllerPitchInput(Value * LookUpScaleFactor);
}

void AMarine::TurnRate(float Rate)
{
	// Calculate delta for this frame from the rate information, in degree per second
	AddControllerYawInput(Rate * CurrentTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMarine::LookUpRate(float Rate)
{
	// Calculate delta for this frame from the rate information, in degree per second
	AddControllerPitchInput(Rate * CurrentLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMarine::StartFiring()
{
	bFirePressed = true;
	FireWeapon();
}

void AMarine::StopFiring()
{
	bFirePressed = false;
	EquippedWeapon->StopFiring();
}

void AMarine::StartAiming()
{
	bAimPressed = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void AMarine::StopAiming()
{
	bAimPressed = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AMarine::CameraAimInterp(float DeltaTime)
{
	if (bAimPressed)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraAimFOV, DeltaTime, CameraAimInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, CameraAimInterpSpeed);
	}

	FollowCamera->SetFieldOfView(CameraCurrentFOV);
}

void AMarine::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		// Get the hand socket for the weapon
		const USkeletalMeshSocket* RightHandSocket = GetMesh()->GetSocketByName("weapon_r");

		if (RightHandSocket)
		{
			// Attach weapon to the socket
			RightHandSocket->AttachActor(WeaponToEquip, GetMesh());
		}

		// Set EquippedWeapon to WeaponToEquip and set item state to Equipped
		EquippedWeapon = WeaponToEquip;
	}
}

AWeapon* AMarine::SpawnDefaultWeapon()
{
	// Check if TSubclassOf is valid
	if (DefaultWeapon)
	{
		// Spawn default weapon based on Weapon BP
		return GetWorld()->SpawnActor<AWeapon>(DefaultWeapon);
	}

	return nullptr;
}

bool AMarine::CrosshairLineTrace(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	// Get current size of the viewport
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get screen space location of the crosshair
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50'000.0f };
		OutHitLocation = End;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Visibility);

		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}

	return false;
}

bool AMarine::GetCrosshairLineTraceEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	// Crosshair line trace hit
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = CrosshairLineTrace(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		// Tentative beam location - still need to trace from gun
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{
		// OutBeamLocation is the End location for the line trace
	}

	// Perform a second line trace from gun barrel
	FHitResult WeaponTraceHit;
	const FVector WeaponLineTraceStart{ MuzzleSocketLocation };
	const FVector WeaponLineTraceStartToEnd{ OutBeamLocation - WeaponLineTraceStart };
	const FVector WeaponLineTraceEnd{ MuzzleSocketLocation + WeaponLineTraceStartToEnd * 1.25f };

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponLineTraceStart, WeaponLineTraceEnd, ECollisionChannel::ECC_Visibility);

	// Is there an object between weapon barrel and beam end point?
	if (WeaponTraceHit.bBlockingHit)
	{
		OutBeamLocation = WeaponTraceHit.Location;

		return true;
	}

	return false;
}

void AMarine::FireWeapon()
{
	if (EquippedWeapon == nullptr) return;

	PlayFireMontage();
	EquippedWeapon->Fire();
}

void AMarine::PlayFireMontage()
{
	// Get AnimBP
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// Play fire anim montage
	if (AnimInstance && FireMontage)
	{
		if (bAimPressed)
		{
			AnimInstance->Montage_Play(FireMontage);
			AnimInstance->Montage_JumpToSection(FName("Fire_Rifle_Ironsights"));
		}
		else
		{
			AnimInstance->Montage_Play(FireMontage);
			AnimInstance->Montage_JumpToSection(FName("Fire_Rifle_Hip"));
		}
	}
}

void AMarine::SendBullet()
{
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetWeaponMesh()->GetSocketByName("GunBarrel");

	if (BarrelSocket)
	{
		const FTransform BarrelSocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetWeaponMesh());
		
		/*if (EquippedWeapon->GetMuzzleFlash())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetMuzzleFlash(), BarrelSocketTransform);
		}*/

		FVector BeamEnd;
		bool bBeamEnd = GetCrosshairLineTraceEndLocation(BarrelSocketTransform.GetLocation(), BeamEnd);

		if (bBeamEnd)
		{
			if (EquippedWeapon->GetBulletHitImpact())
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetBulletHitImpact(), BeamEnd);
			}

			if (EquippedWeapon->GetBulletSmokeTrail())
			{
				UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EquippedWeapon->GetBulletSmokeTrail(), BarrelSocketTransform);

				if (Beam)
				{
					Beam->SetVectorParameter(FName("Target"), BeamEnd);
				}
			}
		}
	}
}

// Called every frame
void AMarine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraAimInterp(DeltaTime);
}

// Called to bind functionality to input
void AMarine::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMarine::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMarine::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AMarine::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMarine::LookUp);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMarine::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMarine::LookUpRate);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMarine::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMarine::StopFiring);

	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AMarine::StartAiming);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AMarine::StopAiming);
}

