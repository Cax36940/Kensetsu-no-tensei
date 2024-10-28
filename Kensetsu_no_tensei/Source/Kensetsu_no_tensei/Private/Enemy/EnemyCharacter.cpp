// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Road/RoadPath.h"
#include "Components/SplineComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BoxComponent and set as RootComponent for the Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	// Create StaticMeshComponent and Attach to BoxComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (PathToFollow)
    {
        accumDistance += Speed * DeltaTime;
        
        FVector NewLocation = PathToFollow->SplineComponent->GetLocationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
        SetActorLocation(NewLocation);
        FRotator SplineRotation = PathToFollow->SplineComponent->GetRotationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
        SetActorRotation(SplineRotation);
    }
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}