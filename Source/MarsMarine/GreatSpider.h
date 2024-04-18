// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GreatSpider.generated.h"

/**
 * 
 */
UCLASS()
class MARSMARINE_API AGreatSpider : public AEnemy
{
	GENERATED_BODY()
	
public:
	AGreatSpider();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player", meta = (AllowPrivateAccess = "true"))
	class AMarine* Marine;

	// Sphere Collision to detect Actor and decide whether or not to attack that Actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
	class USphereComponent* CombatSphere;

	// Boolean to check if this pawn is dead or not
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool IsDead;

	// Montage containing death animations
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* DeathMontage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when an Actor begins overlapping with Combat Sphere Collision
	UFUNCTION()
	void OnCombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Called when an Actor stops overlapping with Combat Sphere Collision
	UFUNCTION()
	void OnCombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Called when this actor die
	void Die();

	UFUNCTION(BlueprintCallable)
	// Called in conjunction with Anim Notify to play Death Pose animation
	void PlayDeathPose();

	// Called to play death animation in a montage
	void PlayDeathMontage();



public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called to receive any damage from Apply Damage 
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
