// Fill out your copyright notice in the Description page of Project Settings.


#include "Potion.h"
#include "MainCharacter.h"

// Sets default values
APotion::APotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}



// Called when the game starts or when spawned
void APotion::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &APotion::OnBeginOverlap);
}

// Called every frame
void APotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APotion::OnBeginOverlap(AActor* PotionActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player")) {
		AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
		Player->PotionPickup();
		this->Destroy();
	}
}