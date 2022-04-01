// Fill out your copyright notice in the Description page of Project Settings.


#include "SO_CharacterMovementComponent.h"

#include "SO_PlayerController.h"

#include "streumon_fpsCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

USO_CharacterMovementComponent::USO_CharacterMovementComponent( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{

}


void USO_CharacterMovementComponent::TryStartWallrun()
{
	FCollisionQueryParams traceParam;
	traceParam.bDebugQuery = true;
	FHitResult hitResultRight;
	FHitResult hitResultLeft;
	FVector startTrace = GetActorLocation();

	// Right

	// Wall detection
	FVector endTraceRight = GetActorLocation() + ( GetCharacterOwner()->GetActorRightVector() * detectionRange );
	bool hitOnRight = GetWorld()->LineTraceSingleByChannel( hitResultRight, startTrace, endTraceRight, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
	
	// Debug purpose
	//DrawDebugLine( GetWorld(), startTrace, endTraceRight, FColor::Red, false, 5.0f, 0, 2.0f );

	bool canSnapToNewWall = eIgnoredWallrunSide != EWallrunType::Right
		|| ( eIgnoredWallrunSide == EWallrunType::Right
			&& UKismetMathLibrary::DegAcos( FVector::DotProduct( wallrunNormalToIgnore, hitResultRight.Normal ) ) >= minWallrunAngle );

	// Wall usable ?
	if ( hitOnRight && canSnapToNewWall && IsValid( GetCharacterOwner()->Controller ) )
	{
		GetCharacterOwner()->Controller->SetIgnoreMoveInput( true );
		//SetMovementMode( EMovementMode::MOVE_Custom, (uint8) ECustomMovementMode::MOVE_Wallrun );
		eWallruning = EWallrunType::Right;
		StopIgnoreWallrunSide();
	}

	// Left

	// Wall detection
	FVector endTraceLeft = GetActorLocation() - ( GetCharacterOwner()->GetActorRightVector() * detectionRange );
	bool hitOnLeft = GetWorld()->LineTraceSingleByChannel( hitResultLeft, startTrace, endTraceLeft, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
	
	// Debug purpose
	//DrawDebugLine( GetWorld(), startTrace, endTraceLeft, FColor::Red, false, 5.0f, 0, 2.0f );

	canSnapToNewWall = eIgnoredWallrunSide != EWallrunType::Left
		|| ( eIgnoredWallrunSide == EWallrunType::Left
			&& UKismetMathLibrary::DegAcos( FVector::DotProduct( wallrunNormalToIgnore, hitResultLeft.Normal ) ) >= minWallrunAngle );

	// Wall usable ?
	if ( hitOnLeft && canSnapToNewWall && IsValid( GetCharacterOwner()->Controller ) )
	{
		GetCharacterOwner()->Controller->SetIgnoreMoveInput( true );
		//SetMovementMode( EMovementMode::MOVE_Custom, (uint8) ECustomMovementMode::MOVE_Wallrun );
		eWallruning = EWallrunType::Left;
		StopIgnoreWallrunSide();
	}
}

void USO_CharacterMovementComponent::Wallrun()
{
	FCollisionQueryParams traceParam;
	traceParam.bDebugQuery = true;
	FHitResult hitResult;
	FVector startTrace = GetActorLocation();
	FVector endTrace;

	if ( eWallruning == EWallrunType::Right )
	{
		endTrace = GetActorLocation() + ( GetCharacterOwner()->GetActorRightVector() * detectionRange );
	}
	else
	{
		endTrace = GetActorLocation() - ( GetCharacterOwner()->GetActorRightVector() * detectionRange );
	}

	bool hitOnWall = GetWorld()->LineTraceSingleByChannel( hitResult, startTrace, endTrace, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
	
	// Debug purpose
	//DrawDebugLine( GetWorld(), startTrace, endTrace, FColor::Red, false, 5.0f, 0, 2.0f );

	if ( hitOnWall )
	{
		wallrunNormalToIgnore = hitResult.Normal;
		float wallrunNormalYaw = hitResult.Normal.Rotation().Yaw;

		// Wallrun speed
		float speed = 500.0f;

		// Direction to run
		float directionToRun = eWallruning == EWallrunType::Right ? wallrunNormalYaw + 90.0f : wallrunNormalYaw - 90.0f;

		// Set character rotation to follow wall
		FRotator wallRunDirection( GetCharacterOwner()->GetActorRotation() );
		wallRunDirection.Yaw = directionToRun;
		GetCharacterOwner()->SetActorRotation( wallRunDirection );

		// Set wallrun speed
		GetCharacterOwner()->GetRootComponent()->ComponentVelocity = FVector( GetCharacterOwner()->GetActorForwardVector().X * wallrunSpeed, GetCharacterOwner()->GetActorForwardVector().Y * wallrunSpeed, GetCharacterOwner()->GetRootComponent()->ComponentVelocity.Z );
		Velocity = FVector( GetCharacterOwner()->GetActorForwardVector().X * wallrunSpeed, GetCharacterOwner()->GetActorForwardVector().Y * wallrunSpeed, Velocity.Z );

		// Fuck the gravity
		GravityScale = 0.0f;

		// Dont go up nor down
		SetPlaneConstraintNormal( FVector::UpVector );

		// Player distance from wall
		GetCharacterOwner()->SetActorLocation( hitResult.ImpactPoint + ( hitResult.Normal * wallrunDistance ), false, ( FHitResult* )nullptr, ETeleportType::TeleportPhysics );
	}
	else
	{
		FallOffWall();
	}
}

void USO_CharacterMovementComponent::FallOffWall()
{
	if ( !IsWallRuning() )
	{
		return;
	}

	GetCharacterOwner()->Controller->SetIgnoreMoveInput( false );

	// Update state
	eWallruning = EWallrunType::None;

	//SetMovementMode( EMovementMode::MOVE_Falling );

	// Make gravity great again
	GravityScale = 1.0f;

	// Allow to move up and down
	SetPlaneConstraintNormal( FVector( 0.0f, 0.0f, 0.0f ) );
}

void USO_CharacterMovementComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	UCharacterMovementComponent::TickComponent( DeltaTime, TickType, ThisTickFunction );

	auto characterOwner_SO = Cast<Astreumon_fpsCharacter>( GetCharacterOwner() );

	if ( IsValid( characterOwner_SO ) && characterOwner_SO->IsWallrunEnable() )
	{
		if ( IsWallRuning() )
		{
			Wallrun();
		}
		else if ( IsFalling() )
		{
			TryStartWallrun();
		}
	}
}

void USO_CharacterMovementComponent::JumpOffWall()
{
	if ( !IsWallRuning() )
	{
		return;
	}

	// Launch character calculation
	IgnoreWallrunSide( eWallruning, ignoreWallTime );

	FVector wallrunLaunchVelocity = eWallruning == EWallrunType::Left ? GetCharacterOwner()->GetActorRightVector() * wallrunLaunchVelocitySide : GetCharacterOwner()->GetActorRightVector() * ( -wallrunLaunchVelocitySide );
	wallrunLaunchVelocity.Z = wallrunLaunchVelocityZ;

	// Character is leaving the wall
	FallOffWall();

	// Launch character
	GetCharacterOwner()->LaunchCharacter( wallrunLaunchVelocity, false, true );
}

void USO_CharacterMovementComponent::IgnoreWallrunSide( EWallrunType wallSide, float timeToIgnore )
{
	eIgnoredWallrunSide = wallSide;
	GetCharacterOwner()->GetWorldTimerManager().SetTimer( ignoreWallTimerHandler, this, &USO_CharacterMovementComponent::StopIgnoreWallrunSide, ignoreWallTime, false, -1.0f );
}

void USO_CharacterMovementComponent::StopIgnoreWallrunSide()
{
	GetCharacterOwner()->GetWorldTimerManager().ClearTimer( ignoreWallTimerHandler );
	eIgnoredWallrunSide = EWallrunType::None;
}

bool USO_CharacterMovementComponent::IsWallRuning() const
{
	//return MovementMode == MOVE_Custom && CustomMovementMode == (uint8)ECustomMovementMode::MOVE_Wallrun;
	return eWallruning != EWallrunType::None;
}

EWallrunType USO_CharacterMovementComponent::GetWallRuningSide() const
{
	return eWallruning;
}