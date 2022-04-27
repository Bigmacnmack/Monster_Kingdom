// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UBoxComponent;

UCLASS()
class MONSTER_KINGDOM_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Rotate(float DeltaTime);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotationRate = 45.0f;

	bool Used;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* AttackHitBox;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults);
};
