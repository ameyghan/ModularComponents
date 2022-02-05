#include "DynamicCamera.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ADynamicCamera::ADynamicCamera()
{
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = CameraRoot;
	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetBoxExtent(FVector(100.f));
	TriggerBox->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	
	BlendTime = 2.0f;
	CameraBlendTimer = 3.0f;
}

void ADynamicCamera::BeginPlay()
{
	Super::BeginPlay();
	
	/** Called when the character steps on the TriggerBox (The box) */
	TriggerBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADynamicCamera::OnOverlapBegin);

	/** Called when the player leaves the TriggerBox */
	TriggerBox->OnComponentEndOverlap.AddUniqueDynamic(this, &ADynamicCamera::OnOverlapEnd);
}

void ADynamicCamera::BlendCameraInAndOut(ACharacter* InCharacter, UCharacterMovementComponent* InCharacterMovement)
{
	if (InCharacterMovement->IsWalking())
	{
		InCharacterMovement->DisableMovement();
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		check(PlayerController);
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetViewTargetWithBlend(this, BlendTime);
		FTimerHandle TimerHandle;
		/** The first lambda (which is for when the player steps on the TriggerBox) 
		 *	can capture the timerhandle by reference to pass it to the second lambda delegate.
		 *	All other values have to be captured by value otherwise you will get a nullptr crash.
		 */
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=, &TimerHandle]
		{
			check(InCharacter);
			PlayerController->SetViewTargetWithBlend(InCharacter, CameraBlendTimer);
			GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([=]
			{
				check(InCharacterMovement && PlayerController);
				InCharacterMovement->SetMovementMode(MOVE_Walking);
				PlayerController->SetIgnoreLookInput(false);
			}), CameraBlendTimer, false);
		}), CameraBlendTimer, false);
	}
}

void ADynamicCamera::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OverlappedCharacter = Cast<ACharacter>(OtherActor);
	UCharacterMovementComponent* CharacterMovement = OverlappedCharacter->GetCharacterMovement();
	check(OverlappedCharacter && CharacterMovement);
	BlendCameraInAndOut(OverlappedCharacter, CharacterMovement);
}

void ADynamicCamera::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Destroy();
}





