// Fill out your copyright notice in the Description page of Project Settings.


#include "SO_Projectile.h"

#include "streumon_fpsCharacter.h"
#include "streumon_fpsGamemode.h"
#include "SO_PlayerController.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ASO_Projectile::ASO_Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>( TEXT( "SphereComp" ) );
	CollisionComp->InitSphereRadius( 5.0f );
	CollisionComp->BodyInstance.SetCollisionProfileName( "Projectile" );
	CollisionComp->OnComponentHit.AddDynamic( this, &ASO_Projectile::OnHit );		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride( FWalkableSlopeOverride( WalkableSlope_Unwalkable, 0.f ) );
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>( TEXT( "ProjectileComp" ) );
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = ProjectileSpeed;
	ProjectileMovement->MaxSpeed = ProjectileSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

}

void ASO_Projectile::OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit )
{
	// Only add impulse and destroy projectile if we hit a physics
	if ( ( OtherActor != nullptr ) && ( OtherActor != this ) && ( OtherComp != nullptr ) )
	{
		if ( auto otherCharacter = Cast<Astreumon_fpsCharacter>( OtherActor ); IsValid( otherCharacter ) )
		{
			if ( Astreumon_fpsCharacter* characterOwner = GetOwner<Astreumon_fpsCharacter>(); IsValid( characterOwner ) )
			{
				if ( ASO_PlayerController* ownerPlayerController = characterOwner->GetController<ASO_PlayerController>(); IsValid( ownerPlayerController ) )
				{
					ownerPlayerController->IncrementScore();
				}
			}
		}
		Destroy();
	}
}