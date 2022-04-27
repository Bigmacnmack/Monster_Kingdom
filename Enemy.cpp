// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "AIModule/Classes/AIController.h"
#include "MainCharacter.h"
#include "Animation/AnimInstance.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Detect Sphere"));
	DetectSphere->SetupAttachment(GetRootComponent());
	DetectSphere->InitSphereRadius(600.0f);

	CombatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Combat Sphere"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.0f);

	AttackHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackHitBox"));
	AttackHitBox->SetupAttachment(GetMesh(), TEXT("HitboxSocket"));

	GetHitSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("GetHitSoundEffect"));
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	AI_Controller = Cast<AAIController>(GetController());

	
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::DetectSphereOnBegingOverlap);
	DetectSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::DetectSphereOnEndOverlap);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnBegingOverlap);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnEndOverlap);

	AttackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AttackHitBoxOnBegingOverlap);

	GetHitSoundEffect->Deactivate();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemy::DetectSphereOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults)
{
	if (!IsAlive)
		return;

	AMainCharacter* MyChar = Cast<AMainCharacter>(OtherActor);

	if (MyChar) {
		TargetChar = MyChar;
		MoveToTarget();
	}
}


void AEnemy::DetectSphereOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsAlive)
		return;

	AMainCharacter* MyChar = Cast<AMainCharacter>(OtherActor);

	if (MyChar) {
		TargetChar = nullptr;
		
		if (AI_Controller)
			AI_Controller->StopMovement();
	}
}


void AEnemy::CombatSphereOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults)
{
	if (!IsAlive)
		return;

	AMainCharacter* MyChar = Cast<AMainCharacter>(OtherActor);

	if (MyChar) {
		bTargetInAttackRange = true;
		float AttackDelay = FMath::RandRange(AttackDelayMin, AttackDelayMax);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackDelay);
		//GetWorldTimerManager를 불러와서 타이머 세팅. 이 객체 안에서, attack 함수를 AttackDelay 후에
	}
}


void AEnemy::CombatSphereOnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsAlive)
		return;

	AMainCharacter* MyChar = Cast<AMainCharacter>(OtherActor);

	if (MyChar) {
		bTargetInAttackRange = false;
	}
}


void AEnemy::AttackHitBoxOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults)
{
	if (CanDetectDamageCollision) {
		AMainCharacter* MyChar = Cast<AMainCharacter>(OtherActor);

		if (MyChar) {
			CanDetectDamageCollision = false;

			UE_LOG(LogTemp, Warning, TEXT("Collided with Player"));

			MyChar->ApplyDamage();
		}
	}

}

void AEnemy::MoveToTarget()
{
	if (bAttacking)
		return;

	if (AI_Controller) {
		FNavPathSharedPtr NavPath; //AI 전용 네비 시스템 설정
		FAIMoveRequest MoveRequest; //AI 전용 네비 시스템에서의 이동 request

		MoveRequest.SetGoalActor(TargetChar); //이동 타겟 설정
		MoveRequest.SetAcceptanceRadius(MoveToTargetRadius); //타겟까지의 거리를 원으로 설정

		AI_Controller->MoveTo(MoveRequest, &NavPath);

	}
}

void AEnemy::Attack()
{
	if (!IsAlive)
		return;

	if (bAttacking)
		return;

	bAttacking = true;
	CanDetectDamageCollision = true;

	if (AI_Controller)
		AI_Controller->StopMovement();

	auto AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.15f);
		AnimInstance->Montage_JumpToSection(TEXT("Attack"), CombatMontage);
	}
}

void AEnemy::AttackEnded()
{
	bAttacking = false;
	CanDetectDamageCollision = false;

	if (TargetChar) {
		if (bTargetInAttackRange) {
			float AttackDelay = FMath::RandRange(AttackDelayMin, AttackDelayMax);
			GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackDelay);
		}
		else {
			MoveToTarget();
		}
	}

	else {
		bTargetInAttackRange = false;
	}
}

void AEnemy::ApplyDamage() //parameter에서 무기 종류에 따라 데미지를 조정해보자
{
	Health -= 20.0f;
	
	GetHitSoundEffect->Activate();
	GetHitSoundEffect->Play();

	if (Health <= 0.0f) {
		Health = 0.0f;
		IsAlive = false;

		if (AI_Controller) {
			AI_Controller->StopMovement();
		}

		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::DisposeEnemy, 2.0f);
	}
}

void AEnemy::DisposeEnemy()
{
	Destroy();
}

