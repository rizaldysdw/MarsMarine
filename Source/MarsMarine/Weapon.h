// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class MARSMARINE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

private:
	// Skeletal Mesh for this weapon
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* WeaponSkeletalMesh;

	// Weapon firing rate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float FireRate;

	// Weapon's bullet damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	float Damage;

	// Weapon shot sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effects", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShotSound;

	// Gun muzzle flash effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effects", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	// Gun impact effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletHitImpact;

	// Gun trail effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletSmokeTrail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponSkeletalMesh; }
	FORCEINLINE float GetWeaponFireRate() const { return FireRate; }
	FORCEINLINE float GetWeaponDamage() const { return Damage; }
	FORCEINLINE USoundCue* GetShotSound() const { return ShotSound; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE UParticleSystem* GetBulletHitImpact() const { return BulletHitImpact; }
	FORCEINLINE UParticleSystem* GetBulletSmokeTrail() const { return BulletSmokeTrail; }
};
