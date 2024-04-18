// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class MARSMARINE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

private:
	// Blackboard Component for the enemy
	UPROPERTY(BlueprintReadWrite, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;

	// Behavior Tree Component for the enemy
	UPROPERTY(BlueprintReadWrite, Category = "Artificial Intelligence", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;


public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComponent() const { return BlackboardComponent; }
};
