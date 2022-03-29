// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "streumon_fpsCharacter.generated.h"

UENUM()
enum EWallrunType
{
	None	UMETA( DisplayName = "None" ),
	Left	UMETA( DisplayName = "LeftWallrun" ),
	Right	UMETA( DisplayName = "RightWallrun" ),
};

UCLASS(config=Game)
class Astreumon_fpsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Is character currently wallruning ? */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Wallrun, meta = ( AllowPrivateAccess = "true" ) )
	TEnumAsByte<EWallrunType> eWallruning = EWallrunType::None;

	/** Side to look for preventing re wallruning */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Wallrun, meta = ( AllowPrivateAccess = "true" ) )
	TEnumAsByte<EWallrunType> eIgnoredWallrunSide = EWallrunType::None;

	/** Normal to look for preventing re wallruning */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Wallrun, meta = ( AllowPrivateAccess = "true" ) )
	FVector wallrunNormalToIgnore = FVector::ZeroVector;

	/** Handle the timer of the ignore wallrun time stamp */
	FTimerHandle ignoreWallTimerHandler;

	/** Range at which the player can snap to a wall to wallrun.
	 *  Range in game unit.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters")
	float detectionRange = 50.0f;

	/** Speed of player when wallruning.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters" )
	float wallrunSpeed = 500.0f;

	/** Height velocity when player jumps off a wall.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters" )
	float wallrunLaunchVelocityZ = 450.0f;

	/** Side velocity when player jumps off a wall.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters" )
	float wallrunLaunchVelocitySide = 450.0f;

	/** Minimum angle between walls to prevent player from continuously jumping on the same wall.
	 *  Angle in degree
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters" )
	float minWallrunAngle = 30.0f;

	/** Time during which a player can't wallrun on a wall with the same direction as the previous one.
	 *  Angle determined by minWallrunAngle.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun | Parameters" )
	float ignoreWallTime = 0.5f;
public:
	Astreumon_fpsCharacter();

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

	/**
	 * Called when the character is in the air to seek for wallrun.
	 * Can be called on ground on specific case if needed.
	 * Left has priority over Right with this.
	 */
	void TryStartWallrun();

	/**
	 * Called when the character is already wall running
	 * @Param wallrunSide  indicate the wall relative position to the player.
	 * @Param wallNormalYaw  Used to get the Z orientation of the wall to determine where the player should wall run
	 */
	void Wallrun();

	/**
	 * Called when the character is leaving the wallrun cycle, could be forced or just exiting the current wall.
	 */
	void FallOffWall();

	/**
	 * Called when the character is jumping off a wall
	 */
	void JumpOffWall();

	void IgnoreWallrunSide( EWallrunType wallSide, float timeToIgnore );

	void StopIgnoreWallrunSide();

public:
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION( BlueprintCallable )
	bool IsWallRuning() const;

	UFUNCTION( BlueprintCallable )
	EWallrunType GatWallRuning() const;

	/**
	 * Handle wallrun jump before deciding or not to apply normal jump mechanic
	 */
	virtual void Jump() override;

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

