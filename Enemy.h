// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/AudioComponent.h"

#include "Enemy.generated.h"

class USphereComponent;
class UBoxComponent;
class AMainCharacter;
class UAnimMontage;
class AAIController;
class AMainCharacter;

UCLASS()
class MONSTER_KINGDOM_API AEnemy : public ACharacter
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere)
		USphereComponent* DetectSphere;

	UPROPERTY(EditAnywhere)
		USphereComponent* CombatSphere;

	UPROPERTY(EditAnywhere)
		UBoxComponent* AttackHitBox;

	UPROPERTY(EditAnywhere)
		UAudioComponent* GetHitSoundEffect;

	UPROPERTY(BlueprintReadOnly)
		bool IsAlive = true;

	UFUNCTION()
		virtual void DetectSphereOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults);

	UFUNCTION()
		virtual void DetectSphereOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex);

	UFUNCTION()
		virtual void CombatSphereOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults);

	UFUNCTION()
		virtual void CombatSphereOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		virtual void AttackHitBoxOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults);

	AMainCharacter* TargetChar;

	AAIController* AI_Controller;

	void MoveToTarget();

	bool bAttacking;

	UPROPERTY(EditAnywhere)
		float MoveToTargetRadius = 10.0f;

	UPROPERTY(EditAnywhere)
		bool bTargetInAttackRange;

	float AttackDelayMin = 0.2f;
	float AttackDelayMax = 0.7f;

	bool CanDetectDamageCollision;

	void Attack();

	UFUNCTION(BlueprintCallable)
		void AttackEnded();

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere)
		UAnimMontage* CombatMontage;

	float Health = 100.0f;

	void ApplyDamage();
	void DisposeEnemy();

};
