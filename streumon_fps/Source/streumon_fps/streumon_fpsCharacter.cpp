// Copyright Epic Games, Inc. All Rights Reserved.

#include "streumon_fpsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Astreumon_fpsCharacter

Astreumon_fpsCharacter::Astreumon_fpsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void Astreumon_fpsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &Astreumon_fpsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &Astreumon_fpsCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &Astreumon_fpsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &Astreumon_fpsCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &Astreumon_fpsCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &Astreumon_fpsCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &Astreumon_fpsCharacter::OnResetVR);
}


void Astreumon_fpsCharacter::OnResetVR()
{
	// If streumon_fps is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in streumon_fps.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void Astreumon_fpsCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void Astreumon_fpsCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void Astreumon_fpsCharacter::TryStartWallrun()
{
	float detectionRange = 50.0f;

	FCollisionQueryParams traceParam;
	traceParam.bDebugQuery = true;
	FHitResult hitResultRight;
	FHitResult hitResultLeft;
	FVector startTrace = GetActorLocation();

	// Right
	if ( eIgnoredWallrunSide != EWallrunType::Right )
	{
		FVector endTraceRight = GetActorLocation() + ( GetActorRightVector() * detectionRange );
		bool hitOnRight = GetWorld()->LineTraceSingleByChannel( hitResultRight, startTrace, endTraceRight, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
		DrawDebugLine( GetWorld(), startTrace, endTraceRight, FColor::Red, false, 5.0f, 0, 2.0f );
		if ( hitOnRight )
		{
			eWallruning = EWallrunType::Right;
			StopIgnoreWallrunSide();
		}
	}

	// Left
	if ( eIgnoredWallrunSide != EWallrunType::Left )
	{
		FVector endTraceLeft = GetActorLocation() - ( GetActorRightVector() * detectionRange );	
		bool hitOnLeft = GetWorld()->LineTraceSingleByChannel( hitResultLeft, startTrace, endTraceLeft, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
		DrawDebugLine( GetWorld(), startTrace, endTraceLeft, FColor::Red, false, 5.0f, 0, 2.0f );
		if ( hitOnLeft )
		{
			eWallruning = EWallrunType::Left;
			StopIgnoreWallrunSide();
		}
	}
	//Left has priority over right
}

void Astreumon_fpsCharacter::Wallrun()
{
	float detectionRange = 50.0f;

	FCollisionQueryParams traceParam;
	traceParam.bDebugQuery = true;
	FHitResult hitResult;
	FVector startTrace = GetActorLocation();
	FVector endTrace;

	if ( eWallruning == EWallrunType::Right )
	{
		endTrace = GetActorLocation() + ( GetActorRightVector() * detectionRange );
	}
	else
	{
		endTrace = GetActorLocation() - ( GetActorRightVector() * detectionRange );
	}
	
	bool hitOnWall = GetWorld()->LineTraceSingleByChannel( hitResult, startTrace, endTrace, ECollisionChannel::ECC_Visibility, traceParam, FCollisionResponseParams::DefaultResponseParam );
	DrawDebugLine( GetWorld(), startTrace, endTrace, FColor::Red, false, 5.0f, 0, 2.0f );

	if ( hitOnWall )
	{
		float wallrunNormalYaw = hitResult.Normal.Rotation().Yaw;

		// Wallrun speed
		float speed = 500.0f;

		// Direction to run
		float directionToRun = eWallruning == EWallrunType::Right ? wallrunNormalYaw + 90.0f : wallrunNormalYaw - 90.0f;

		// Set character rotation to follow wall
		FRotator wallRunDirection( GetActorRotation() );
		wallRunDirection.Yaw = directionToRun;
		SetActorRotation( wallRunDirection );

		// Set wallrun speed
		GetRootComponent()->ComponentVelocity = FVector( GetActorForwardVector().X * speed, GetActorForwardVector().Y * speed, GetRootComponent()->ComponentVelocity.Z );
		GetCharacterMovement()->Velocity = FVector( GetActorForwardVector().X * speed, GetActorForwardVector().Y * speed, GetCharacterMovement()->Velocity.Z );

		// Fuck the gravity
		GetCharacterMovement()->GravityScale = 0.0f;

		// Dont go up nor down
		GetCharacterMovement()->SetPlaneConstraintNormal( FVector( 0.0f, 0.0f, 1.0f ) );
	}
	else
	{
		FallOffWall();
	}
}

void Astreumon_fpsCharacter::FallOffWall()
{
	// Update state
	eWallruning = EWallrunType::None;

	// Make gravity great again
	GetCharacterMovement()->GravityScale = 1.0f;

	// Allow to move up and down
	GetCharacterMovement()->SetPlaneConstraintNormal( FVector( 0.0f, 0.0f, 0.0f ) );
}

void Astreumon_fpsCharacter::JumpOffWall()
{
	if ( !IsWallRuning() )
	{
		return;
	}

	// Launch character cqlculqtion
	IgnoreWallrunSide( eWallruning, 1.0f );
	float wallrunLaunchVelocityZ = 450;
	FVector wallrunLaunchVelocity = eWallruning == EWallrunType::Right ? GetActorRightVector() * 450.0f : GetActorRightVector() * ( -450.0f );
	wallrunLaunchVelocity.Z = wallrunLaunchVelocityZ;

	// Character is leaving the wall
	FallOffWall();

	// Launch character
	LaunchCharacter( wallrunLaunchVelocity, false, true );
}

void Astreumon_fpsCharacter::IgnoreWallrunSide( EWallrunType wallSide, float timeToIgnore )
{
	eIgnoredWallrunSide = wallSide;
	GetWorldTimerManager().SetTimer( ignoreWallTimerHandler, this, &Astreumon_fpsCharacter::StopIgnoreWallrunSide, 0.0f, false, 5.0f );
}

void Astreumon_fpsCharacter::StopIgnoreWallrunSide()
{
	GetWorldTimerManager().ClearTimer( ignoreWallTimerHandler );
	eIgnoredWallrunSide = EWallrunType::None;
}

void Astreumon_fpsCharacter::Tick( float DeltaSeconds )
{
	// Character in air ?
	if ( GetCharacterMovement()->IsFalling() )
	{
		if ( eWallruning == None )
		{
			TryStartWallrun();
		}
	}
	if ( eWallruning != None )
	{
		Wallrun();
	}
}

bool Astreumon_fpsCharacter::IsWallRuning() const
{
	return eWallruning != EWallrunType::None;
}

EWallrunType Astreumon_fpsCharacter::GatWallRuning() const
{
	return eWallruning;
}

void Astreumon_fpsCharacter::Jump()
{
	if ( !IsWallRuning() )
	{
		ACharacter::Jump();
	}
	else
	{
		JumpOffWall();
	}
}

void Astreumon_fpsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void Astreumon_fpsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void Astreumon_fpsCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Astreumon_fpsCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
