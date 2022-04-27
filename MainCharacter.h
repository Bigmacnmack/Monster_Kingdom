// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Components/AudioComponent.h"

#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class UAnimMontage;

UENUM(BlueprintType)
enum class EMovementState : uint8 {
	MS_Normal UMETA(DisplayName="Normal"),
	MS_Sprinting UMETA(DisplayName="Sprinting"),
};

UCLASS()
class MONSTER_KINGDOM_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* Cameraboom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
		UAudioComponent* SoundEffect;

	void MoveForward(float value);
	void MoveRight(float value);
	virtual void Jump() override;

	void SprintKeyDown();
	void SprintKeyUp();

	void LMBDown();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bIsAttacking;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsAlive = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) //enum에 따른 상태 변화 옵션 생성
		EMovementState MovementState;

	UPROPERTY(BlueprintReadOnly)
		AWeapon* EquippedWeapon;
	
	UPROPERTY(BlueprintReadOnly)
		float MovementSpeed;

	UPROPERTY(BlueprintReadOnly)
		bool bInAir;

	void SetMovementSpeedAndAir();

	
	float RunningSpeed = 600.0f;
	float SprintingSpeed = 950.0f;

	void EquipWeapon(AWeapon* WeaponActor);
	
	UPROPERTY(EditAnywhere)
		UAnimMontage* CombatMontage;

	UFUNCTION()
		virtual void DamageBoxOnBegingOverlap(UPrimitiveComponent* OverlappedComp, 
			AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults);

	UPROPERTY(BlueprintReadWrite)
		bool CanDetectDamageCollision;

	UPROPERTY(BlueprintReadWrite)
		float Health = 100.0f;

	void ApplyDamage();
	void RestartGame();
	void PotionPickup();

};
