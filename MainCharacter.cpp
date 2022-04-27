// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/PlayerController.h"
#include "Animation/AnimInstance.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Cameraboom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Cameraboom"));
	Cameraboom->SetupAttachment(GetRootComponent());
	Cameraboom->TargetArmLength = 600.0f;
	Cameraboom->bUsePawnControlRotation = true; //enable cameraboom rotate not character.

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(Cameraboom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	SoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SoundEffect"));
	
	
	//카메라 회전과 캐릭터의 회전 동기화를 끄자. YAW는 기본적으로 설정되어있다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;



	//add character movement settings
	GetCharacterMovement()->bOrientRotationToMovement = true; //rotate character to the direction of the movement
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 550.0f; //점프 높이
	GetCharacterMovement()->AirControl = 0.2f; //공중에서의 움직임 정도 

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	SoundEffect->Deactivate();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetMovementSpeedAndAir();

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMainCharacter::Jump);
	
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMainCharacter::SprintKeyUp);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMainCharacter::SprintKeyDown);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBDown);

}

void AMainCharacter::MoveForward(float Value)
{
	if (!IsAlive)
		return;

	if (Value != 0.0f && !bIsAttacking) {
		auto YawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f); //방향을 얻기 위해 Z축의 회전값을 구하자
		auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); //절대축X(상하 이동)을 기준으로 방향을 잡자

		AddMovementInput(Direction, Value); //절대축에서 Value만큼 이동
	}
}

void AMainCharacter::MoveRight(float Value)
{
	if (!IsAlive)
		return;

	if (!IsAlive)
		return;

	if (Value != 0.0f && !bIsAttacking) {
		auto YawRotation = FRotator(0.0f, Controller->GetControlRotation().Yaw, 0.0f); //방향을 얻기 위해 Z축의 회전값을 구하자
		auto Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); //절대축Y(좌우 이동)을 기준으로 방향을 잡자

		AddMovementInput(Direction, Value); //절대축에서 Value만큼 이동
	}
}

void AMainCharacter::Jump()
{
	if (!IsAlive)
		return;

	if (bIsAttacking)
		return;

	Super::Jump();
}

void AMainCharacter::SprintKeyDown()
{
	if (!IsAlive)
		return;

	if (GetCharacterMovement()->Velocity.Size() == 0) // return when not moving
		return;

	MovementState = EMovementState::MS_Sprinting;
	GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
}

void AMainCharacter::SprintKeyUp()
{
	if (!IsAlive)
		return;

	MovementState = EMovementState::MS_Normal;
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AMainCharacter::LMBDown()
{
	if (!IsAlive)
		return;

	if (EquippedWeapon) {
		UAnimInstance* MyAnim = GetMesh()->GetAnimInstance();

		if (MyAnim && CombatMontage) {
			MyAnim->Montage_Play(CombatMontage, 2.5f); //2.5초 재생
			SoundEffect->Activate();
			SoundEffect->Play();

			float RandAnim = FMath::RandRange(0, 1);

			if (RandAnim > 0) {
				MyAnim->Montage_JumpToSection(TEXT("Attack_1"));
			}
			else {
				MyAnim->Montage_JumpToSection(TEXT("Attack_2"));
			}

			CanDetectDamageCollision = true;

		}
	}

}

void AMainCharacter::SetMovementSpeedAndAir()
{
	auto CharSpeed = GetVelocity();
	auto LateralSpeed = FVector(CharSpeed.X, CharSpeed.Y, 0.0f);

	MovementSpeed = LateralSpeed.Size(); //벡터의 내적을 리턴

	bInAir = GetMovementComponent()->IsFalling();
}

void AMainCharacter::EquipWeapon(AWeapon* WeaponActor)
{
	if (EquippedWeapon != nullptr) {
		return;
	}

	WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("RightHandSocket"));

	WeaponActor->Used = true;

	EquippedWeapon = WeaponActor;


	//add on component begin overlap for hit box
	EquippedWeapon->AttackHitBox->OnComponentBeginOverlap.AddDynamic(this,
		&AMainCharacter::DamageBoxOnBegingOverlap);
}

void AMainCharacter::DamageBoxOnBegingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bSweepFrom, const FHitResult& SweepResults)
{
	if (CanDetectDamageCollision) {
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);

		if (Enemy) {
			CanDetectDamageCollision = false;
			UE_LOG(LogTemp, Warning, TEXT("collided with enemy"));
			Enemy->ApplyDamage();			
		}
	}
}

void AMainCharacter::ApplyDamage()
{
	if (!IsAlive)
		return;

	Health -= 10.0f;

	if (Health <= 0.0f) {
		IsAlive = false;

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainCharacter::RestartGame, 3.0f);
	}
}

void AMainCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void AMainCharacter::PotionPickup()
{
	if (Health < 100.0f) {
		Health = 100.0f;
	}

}

