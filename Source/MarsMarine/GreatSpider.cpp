// Fill out your copyright notice in the Description page of Project Settings.


#include "GreatSpider.h"
#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "Marine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AGreatSpider::AGreatSpider()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Combat Sphere Collision
	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Range Sphere Collision"));
	CombatSphere->SetupAttachment(GetRootComponent());

	MaxHealth = 100.f;
}

void AGreatSpider::BeginPlay()
{
	Super::BeginPlay();

	// Get AI Controller
	EnemyController = Cast<AEnemyController>(GetController());

	if (EnemyController)
	{
		AActor* MarineActor = UGameplayStatics::GetActorOfClass(GetWorld(), AMarine::StaticClass());

		Marine = Cast<AMarine>(MarineActor);

		EnemyController->GetBlackboardComponent()->SetValueAsObject("Marine", Marine);

		EnemyController->RunBehaviorTree(BehaviorTree);
	}

	// Set Health
	Health = MaxHealth;

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AGreatSpider::OnCombatSphereBeginOverlap);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AGreatSpider::OnCombatSphereEndOverlap);

	// Set Mesh collision to block Visbility which Player line trace uses,
	// this allow line trace to hit this Actor Mesh 
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
}

void AGreatSpider::OnCombatSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto MarineCharacter = Cast<AMarine>(OtherActor);

	if (MarineCharacter)
	{
		GetCharacterMovement()->Activate(false);
	}
}

void AGreatSpider::OnCombatSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == nullptr) return;

	auto MarineCharacter = Cast<AMarine>(OtherActor);

	if (MarineCharacter)
	{
		GetCharacterMovement()->Activate(true);
	}
}

void AGreatSpider::Die()
{
	if (!IsDead)
	{
		IsDead = true;
		PlayDeathMontage();
	}
}

void AGreatSpider::PlayDeathPose()
{
	GetMesh()->bPauseAnims = true;
}

void AGreatSpider::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Play(DeathMontage);
		AnimInstance->Montage_JumpToSection("Death", DeathMontage);
	}
}

void AGreatSpider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGreatSpider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

}

float AGreatSpider::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (Health - DamageAmount <= 0.0f)
	{
		Health = 0.0f;
		Die();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}