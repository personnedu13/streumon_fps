// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SO_CharacterMovementComponent.h"
#include "streumon_fpsCharacter.generated.h"

UCLASS(config=Game)
class Astreumon_fpsCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = ( AllowPrivateAccess = "true" ) )
	class UCameraComponent* FollowCamera;

	/** Is character allowed to wallrun */
	UPROPERTY( BlueprintReadOnly, Category = "Wallrun", meta = ( AllowPrivateAccess = "true" ) )
	bool wallrunEnable = true;

	/** Is the character allowed to have aim assist */
	UPROPERTY( BlueprintReadOnly, Category = "Aim assist", meta = ( AllowPrivateAccess = "true" ) )
	bool aimAssistEnable = true;

	/** Aim assist radius
	 *  How far from the target do we need to be to start aiming at it.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Aim assist", meta = ( AllowPrivateAccess = "true" ) )
	float aimAssistRadius = 100.0f;

	/** Aim assist range.
	 *  If a target is further than this range, it will not lock on it.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Aim assist", meta = ( AllowPrivateAccess = "true" ) )
	float aimAssistRange = 10000.0f;

	/** Aim assist yaw adjust.
	 *  How much of the difference between sight and target center should be adjusted (Horizontal).
	 *  Value between 0 and 1, 0 : no adjust, 1 : insta lock.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Aim assist", meta = ( AllowPrivateAccess = "true" ) )
	float aimAssistYawAdjust = 0.2f;

	/** Aim assist pitch adjust.
	 *  How much of the difference between sight and target center should be adjusted (Vertical).
	 *  Value between 0 and 1, 0 : no adjust, 1 : insta lock.
	 */
	UPROPERTY( EditDefaultsOnly, BlueprintReadOnly, Category = "Aim assist", meta = ( AllowPrivateAccess = "true" ) )
	float aimAssistPitchAdjust = 0.2f;

public:
	Astreumon_fpsCharacter( const FObjectInitializer& ObjectInitializer);

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:
	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	APawn* LookForTarget();

	/**
	* Called when the character is jumping off a wall
	*/
	UFUNCTION( Server, Reliable, WithValidation )
	void JumpOffWallServer();
	virtual void JumpOffWallServer_Implementation();
	virtual bool JumpOffWallServer_Validate() { return true; };

public:

	UFUNCTION( Server, Reliable/*, WithValidation*/)
	void ServerSwitchWallrun();
	virtual void ServerSwitchWallrun_Implementation();
	virtual bool ServerSwitchWallrun_Validate() { return true; };

	/**
	 * Handle wallrun jump before deciding or not to apply normal jump mechanic
	 */
	virtual void Jump() override;

	/**
	 * Used to switch the activation of the wallrun ability.
	 */
	virtual void SwitchWallrun();

	/**
	 * Used to switch the activation of the aimAssist ability.
	 */
	virtual void SwitchAimAssist();

	virtual void Tick( float DeltaSeconds ) override;

	// GETTER 

	UFUNCTION( BlueprintCallable )
	bool IsWallrunEnable() const;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

