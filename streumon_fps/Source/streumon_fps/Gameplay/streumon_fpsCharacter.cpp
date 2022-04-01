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
#include "Kismet/KismetMathLibrary.h"
#include "SO_PlayerController.h"

//////////////////////////////////////////////////////////////////////////
// Astreumon_fpsCharacter

Astreumon_fpsCharacter::Astreumon_fpsCharacter( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer.SetDefaultSubobjectClass<USO_CharacterMovementComponent>( ACharacter::CharacterMovementComponentName ) )
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

	// Aim assist
	PlayerInputComponent->BindAction( "AimAssist", IE_Pressed, this, &Astreumon_fpsCharacter::SwitchAimAssist );

	// Wallrun
	PlayerInputComponent->BindAction( "Wallrun", IE_Pressed, this, &Astreumon_fpsCharacter::SwitchWallrun );

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

APawn* Astreumon_fpsCharacter::LookForTarget()
{
	FHitResult hitResult;
	//FVector startTrace = GetActorLocation();
	//FVector endTrace = GetActorLocation() + ( GetActorForwardVector() * aimAssistRange );

	FVector startTrace = FollowCamera->GetComponentLocation();
	FVector endTrace = FollowCamera->GetComponentLocation() + ( FollowCamera->GetForwardVector() * aimAssistRange );

	static FName CollisionQueryName = TEXT( "AimAssistQuery" );

	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.AddIgnoredActor( this );
	
	FCollisionShape shape = FCollisionShape::MakeSphere( aimAssistRadius );

	if ( GetWorld()->SweepSingleByChannel( hitResult, startTrace, endTrace, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, shape, collisionQueryParams ) )
	{
		// Debug purpose
		//DrawDebugSphere( GetWorld(), hitResult.Location, 10.0f, 10, FColor::Green, false, 5.0f, 0U, 2.0f );
		
		FRotator cameraToTarget = (hitResult.GetActor()->GetActorLocation() - startTrace).Rotation();
		FRotator cameraToImpactPoint = ( hitResult.ImpactPoint - startTrace ).Rotation();

		// Use controller input to blend with player input.
		AddControllerPitchInput( ( cameraToImpactPoint.Pitch - cameraToTarget.Pitch ) * aimAssistPitchAdjust );
		AddControllerYawInput( ( cameraToTarget.Yaw - cameraToImpactPoint.Yaw ) * aimAssistYawAdjust );
	}

	return nullptr;
}

void Astreumon_fpsCharacter::JumpOffWallServer_Implementation()
{
	auto SO_movementComponent = Cast<USO_CharacterMovementComponent>( GetMovementComponent() );
	SO_movementComponent->JumpOffWall();
}

void Astreumon_fpsCharacter::ServerSwitchWallrun_Implementation()
{
	wallrunEnable = !wallrunEnable;
	if ( auto playerController = GetController<ASO_PlayerController>(); IsValid( playerController ) )
	{
		playerController->OnWallrunStateReceived( wallrunEnable );
	}
	auto SO_movementComponent = Cast<USO_CharacterMovementComponent>( GetMovementComponent() );
	if ( IsValid( SO_movementComponent ) && !wallrunEnable )
	{
		SO_movementComponent->FallOffWall();
	}
}

void Astreumon_fpsCharacter::ServerSwitchAimAssist_Implementation()
{
	aimAssistEnable = !aimAssistEnable;
	if ( auto playerController = GetController<ASO_PlayerController>(); IsValid( playerController ) )
	{
		playerController->OnAimAssistStateReceived( aimAssistEnable );
	}
}

void Astreumon_fpsCharacter::Jump()
{
	auto SO_movementComponent = Cast<USO_CharacterMovementComponent>( GetMovementComponent() );
	if ( SO_movementComponent != nullptr )
	{
		if ( !IsWallrunEnable() || !SO_movementComponent->IsWallRuning() )
		{
			ACharacter::Jump();
		}
		else
		{
			JumpOffWallServer();
			SO_movementComponent->JumpOffWall();
		}
	}
}

void Astreumon_fpsCharacter::SwitchWallrun()
{
	ServerSwitchWallrun();

	if ( !HasAuthority() )
	{
		wallrunEnable = !wallrunEnable;
		if ( auto playerController = GetController<ASO_PlayerController>(); IsValid( playerController ) )
		{
			playerController->OnWallrunStateReceived( wallrunEnable );
		}
		auto SO_movementComponent = Cast<USO_CharacterMovementComponent>( GetMovementComponent() );
		if ( IsValid( SO_movementComponent ) && !wallrunEnable )
		{
			SO_movementComponent->FallOffWall();
		}
	}
}

bool Astreumon_fpsCharacter::IsWallrunEnable() const
{
	return wallrunEnable;
}

void Astreumon_fpsCharacter::SwitchAimAssist()
{
	ServerSwitchAimAssist();

	if ( !HasAuthority() )
	{
		aimAssistEnable = !aimAssistEnable;
		if ( auto playerController = GetController<ASO_PlayerController>(); IsValid( playerController ) )
		{
			playerController->OnAimAssistStateReceived( aimAssistEnable );
		}
	}
}

void Astreumon_fpsCharacter::Tick( float DeltaSeconds )
{
	ACharacter::Tick( DeltaSeconds );

	if ( aimAssistEnable )
	{
		LookForTarget();
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
