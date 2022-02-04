#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicCamera.generated.h"

class UCharacterMovementComponent;
class APlayerController;

UCLASS()
class /*YOURGAME_API*/ ADynamicCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Constructor */
	ADynamicCamera();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(BlueprintCallable)
	void BlendCameraInAndOut(ACharacter* InCharacter, UCharacterMovementComponent* InCharacterMovement);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	/** Components */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USceneComponent* CameraRoot;
	// The component used to trigger the camera 
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* TriggerPad;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UCameraComponent* Camera;
	
	// The amount of blend time for the camera movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers", meta = (AllowPrivateAccess = "true"))
	float BlendTime;
	
	// The amount of time it takes for the delay to reset the camera back to its original position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers", meta = (AllowPrivateAccess = "true"))
	float CameraBlendTimer;
};
