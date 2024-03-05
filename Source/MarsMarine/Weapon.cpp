// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Weapon Mesh subobject and set it as root
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponSkeletalMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::PlayShotSound()
{
	if (ShotSound)
	{
		UGameplayStatics::PlaySound2D(this, ShotSound);
	}
}

void AWeapon::PlayShotEndSound()
{
	if (ShotEndSound)
	{
		UGameplayStatics::PlaySound2D(this, ShotEndSound);
	}
}

void AWeapon::PlayMuzzleFlashEffect()
{
	const USkeletalMeshSocket* BarrelSocket = WeaponSkeletalMesh->GetSocketByName("GunBarrel");

	if (BarrelSocket)
	{
		const FTransform BarrelSocketTransform = BarrelSocket->GetSocketTransform(WeaponSkeletalMesh);

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, BarrelSocketTransform);
		}
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	PlayShotSound();
	PlayMuzzleFlashEffect();
}

void AWeapon::StopFiring()
{
	PlayShotEndSound();
}

