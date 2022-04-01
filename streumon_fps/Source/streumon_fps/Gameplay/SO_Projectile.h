// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SO_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS( config = Game )
class STREUMON_FPS_API ASO_Projectile : public AActor
{
	
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY( VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = ( AllowPrivateAccess = "true" ) )
	UProjectileMovementComponent* ProjectileMovement;

	/** Projectile speed (and max speed) */
	UPROPERTY( EditAnywhere, Category = Projectile )
	float ProjectileSpeed = 5000.0f;

public:
	ASO_Projectile();

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit( UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
