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

	// Gun shot sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	class USoundCue* ShotSound;

	// Gun shot end sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* ShotEndSound;

	// Gun muzzle flash effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;

	// Gun impact effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletHitImpact;

	// Gun trail effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BulletSmokeTrail;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayShotSound();

	void PlayShotEndSound();

	void PlayMuzzleFlashEffect();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Fire();

	void StopFiring();

public:
	FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() const { return WeaponSkeletalMesh; }
	FORCEINLINE UParticleSystem* GetMuzzleFlash() const { return MuzzleFlash; }
	FORCEINLINE UParticleSystem* GetBulletHitImpact() const { return BulletHitImpact; }
	FORCEINLINE UParticleSystem* GetBulletSmokeTrail() const { return BulletSmokeTrail; }
};
