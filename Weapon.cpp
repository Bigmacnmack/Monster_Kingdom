// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"

#include "MainCharacter.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	AttackHitBox= CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnOverlapBegin);


}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Rotate(DeltaTime);
}

void AWeapon::Rotate(float DeltaTime)
{
	if (Used)
		return;

	auto Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * RotationRate;
	SetActorRotation(Rotation);
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults)
{
	if (OtherActor->ActorHasTag("Player")) {

		AMainCharacter* PlayerChar = Cast<AMainCharacter>(OtherActor);

		if (PlayerChar) {

			PlayerChar->EquipWeapon(this);
		}

	}

}
