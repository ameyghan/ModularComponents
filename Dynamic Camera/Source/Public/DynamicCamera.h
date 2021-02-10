#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "DynamicCamera.generated.h"

class UCharacterMovementComponent;
class USceneComponent;
class UBoxComponent;
class UCameraComponent;
class ACharacter;

UCLASS()
class HOOP_API ADynamicCamera : public AActor
{
	GENERATED_BODY()

public:
	/*constructor*/
	ADynamicCamera();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* CameraRoot;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent* OverlapComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY()
	ACharacter* EpicCharacter;
	UPROPERTY(VisibleAnywhere, Category = "Timers")
	float BlendTime;
	UPROPERTY(VisibleAnywhere, Category = "Timers")
	float CameraResetTimer;
	UPROPERTY(VisibleAnywhere, Category = "Timers")
	float MovementResetTimer;

	UFUNCTION()
	void ResetCamera();	
	UFUNCTION()
	void EnableMovement();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor, UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep,
						const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};