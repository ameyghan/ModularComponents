#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "DynamicCamera.generated.h"

UCLASS()
class /*YOUR_PROJECT_NAME_API*/ ADynamicCamera : public AActor
{
	GENERATED_BODY()

public:
	/*Constructor*/
	ADynamicCamera();
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USceneComponent* CameraRoot;
	
	// The component used to trigger the camera 
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* OverlapComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;

	// The reference character that interacts with the camera to activate the OverlapComponent
	UPROPERTY()
	class ACharacter* EpicCharacter;

	// The amount of blend time for the camera movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers", meta = (AllowPrivateAccess = "true"))
	float BlendTime;
	
	// The amount of time it takes for the delay to reset the camera back to its original position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers", meta = (AllowPrivateAccess = "true"))
	float CameraResetTimer;

	// Resetting the character movement after the cut-scene-like camera movement is done
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers", meta = (AllowPrivateAccess = "true"))
	float MovementResetTimer;

	UFUNCTION(BlueprintCallable)
	void ResetCamera();	
	UFUNCTION(BlueprintCallable)
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

};
