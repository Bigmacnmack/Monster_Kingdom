// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Potion.generated.h"

UCLASS()
class MONSTER_KINGDOM_API APotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APotion();

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
		UCapsuleComponent* CollisionComponent;

	UFUNCTION()
		void OnBeginOverlap(AActor* PotionActor, AActor* OtherActor);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
