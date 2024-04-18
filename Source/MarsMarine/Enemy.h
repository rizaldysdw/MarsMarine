// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MARSMARINE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// AI Controller for this pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class AEnemyController* EnemyController;

	// Behavior Tree for this pawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float Health;

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	FORCEINLINE UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
};
