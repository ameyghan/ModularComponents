#include "DynamicCamera.h"
#include "Components/BoxComponent.h"
#include "components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"

ADynamicCamera::ADynamicCamera()
{
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = CameraRoot;
	
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(100.f));
	OverlapComponent->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());

	BlendTime = 2.0f;
	CameraResetTimer = 3.0f;
	MovementResetTimer = 6.0f;
}

void ADynamicCamera::BeginPlay()
{
	Super::BeginPlay();
	
	/** called when the character steps on the OverlapComponent (The box) */
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ADynamicCamera::OnOverlapBegin);
	
	/** called when the player leaves the OverlapComponent */
	OverlapComponent->OnComponentEndOverlap.AddDynamic(this, &ADynamicCamera::OnOverlapEnd);
}

/** Used to Reset the camera back to the player after the initial SetViewTragetWithBlend */
void ADynamicCamera::ResetCamera()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetCamera() called"));
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	Controller->SetViewTargetWithBlend(EpicCharacter, CameraResetTimer);
}

/** Enables movement once the blend is over and the camera is positioned where the player is */
void ADynamicCamera::EnableMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("EnableMovement() called"));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	UCharacterMovementComponent* PlayerMovement = EpicCharacter->GetCharacterMovement();
	PlayerMovement->SetMovementMode(MOVE_Walking);
	PlayerMovement = Cast<UCharacterMovementComponent>(PlayerController);
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast Successful"));
		PlayerController->SetIgnoreLookInput(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast Failed"));
	}
}

void ADynamicCamera::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
									AActor* OtherActor, 
									UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex, bool bFromSweep,
									const FHitResult& SweepResult)
{
	/**
	 *	Reference to the character (can be any native or blueprint character)  
	 *	cast it to the otherActor to call the onOverlapBegin function on.
	 */
	EpicCharacter = Cast<ACharacter>(OtherActor);
	if (EpicCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Triggered through OnOverlapBegin()"));
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		{
			/**	
			 *	Disabling character movement and changing the camera view to wherever 
			 *	this Actors "Camera" component is with a blend. 
			 *	Do not use DisableInput. Using DisableINput will retain the player's 
			 *	movement speed before the cut scene, so if you were running, after the cut scene
			 *	your character will continue running and that's not common game behavior.
			 */
			UCharacterMovementComponent* CharacterMovement = EpicCharacter->GetCharacterMovement();
			if (CharacterMovement->IsWalking())
			{	
				CharacterMovement->DisableMovement();
				CharacterMovement = Cast<UCharacterMovementComponent>(PlayerController);
				if (PlayerController)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cast Successful"));
					PlayerController->SetIgnoreLookInput(true);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failure"));
				}
			}
			PlayerController->SetViewTargetWithBlend(this, BlendTime);
			FTimerHandle CameraTimerHandle;
			GetWorldTimerManager().SetTimer(CameraTimerHandle, this, &ADynamicCamera::ResetCamera, CameraResetTimer);
			FTimerHandle MovementTimerHandle;
			GetWorldTimerManager().SetTimer(MovementTimerHandle, this, &ADynamicCamera::EnableMovement, MovementResetTimer);
		}
	}
}

void ADynamicCamera::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Object Destroyed through OnOverlapEnd()"));
	Destroy();
}