// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SO_CharacterMovementComponent.generated.h"

UENUM()
enum EWallrunType
{
	None	UMETA( DisplayName = "None" ),
	Left	UMETA( DisplayName = "LeftWallrun" ),
	Right	UMETA( DisplayName = "RightWallrun" )
};

/** Used to declare a custom movement type
 *  TO DO : Implement proper wallrun state, need to rewrite the entire movement component functions, too long.
 */
/*UENUM()
enum class ECustomMovementMode : uint8
{
	MOVE_Default = 0U	UMETA( DisplayName = "Default" ),
	MOVE_Wallrun = 1U	UMETA( DisplayName = "Wallrun" )
};*/

UCLASS()
class STREUMON_FPS_API USO_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_UCLASS_BODY()

	// ATTRIBUTES
protected:
	/** Is character currently wallruning ? */
	UPROPERTY( BlueprintReadOnly, Category = "Wallrun", meta = ( AllowPrivateAccess = "true" ))
	TEnumAsByte<EWallrunType> eWallruning = EWallrunType::None;

	/** Side to look for preventing re wallruning */
	UPROPERTY( BlueprintReadOnly, Category = "Wallrun", meta = ( AllowPrivateAccess = "true" ) )
	TEnumAsByte<EWallrunType> eIgnoredWallrunSide = EWallrunType::None;

	/** Normal to look for preventing re wallruning */
	UPROPERTY( BlueprintReadOnly, Category = "Wallrun", meta = ( AllowPrivateAccess = "true" ) )
	FVector wallrunNormalToIgnore = FVector::ZeroVector;

	/** Handle the timer of the ignore wallrun time stamp */
	FTimerHandle ignoreWallTimerHandler;

	/** Max distance between a player and a wall to allow a player to wallrun */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float detectionRange = 50.0f;

	/** Speed of player when wallruning.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float wallrunSpeed = 500.0f;

	/** Height velocity when player jumps off a wall.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float wallrunLaunchVelocityZ = 450.0f;

	/** Side velocity when player jumps off a wall.
	 *  Speed in game unit / s.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float wallrunLaunchVelocitySide = 450.0f;

	/** Minimum angle between walls to prevent player from continuously jumping on the same wall.
	 *  Angle in degree
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float minWallrunAngle = 30.0f;

	/** Time during which a player can't wallrun on a wall with the same direction as the previous one.
	 *  Angle determined by minWallrunAngle.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float ignoreWallTime = 0.5f;

	/** Distance between character and wall while wall runing.
	 *  Careful to not overlap hitboxes.
	 */
	UPROPERTY( EditDefaultsOnly, Category = "Wallrun|Parameters" )
	float wallrunDistance = 50.0f;

public:

	// METHODS
protected:
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
	 * Called when the character is leaving the wallrun cycle, could be forced, jump off wall or just exiting the current wall.
	 */
	void FallOffWall();

	/**
	 * Called to ignore walls on a side for a specific amount of time.
	 * minWallrunAngle to determine at wich angle difference it will stop ignoring the side.
	 */
	void IgnoreWallrunSide( EWallrunType wallSide, float timeToIgnore );

	/**
	 * Called to stop ignoring walls on a side.
	 */
	void StopIgnoreWallrunSide();
public:
	virtual void TickComponent( float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	
	/**
	* Called when the character is jumping off a wall
	*/
	void JumpOffWall();

	// GETTER

	UFUNCTION( BlueprintCallable )
	bool IsWallRuning() const;

	UFUNCTION( BlueprintCallable )
	EWallrunType GetWallRuningSide() const;
};
