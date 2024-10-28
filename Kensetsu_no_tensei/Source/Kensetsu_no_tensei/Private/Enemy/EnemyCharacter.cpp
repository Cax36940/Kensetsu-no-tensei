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
	
	FActorSpawnParameters SpawnParams;

	PathToFollow = GetWorld()->SpawnActor<ARoadPath>(RoadPathClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
}

void AEnemyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (PathToFollow) {
		PathToFollow->Destroy();
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
    if (PathToFollow)
    {
		float SplineLength = PathToFollow->SplineComponent->GetSplineLength();
		FVector EndLocation = PathToFollow->SplineComponent->GetLocationAtDistanceAlongSpline(SplineLength, ESplineCoordinateSpace::World);
        
		accumDistance += Speed * DeltaTime;
        
        FVector NewLocation = PathToFollow->SplineComponent->GetLocationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
        SetActorLocation(NewLocation + FVector(0, 0, 150));
        FRotator SplineRotation = PathToFollow->SplineComponent->GetRotationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
        SetActorRotation(SplineRotation);

		if (FVector::Dist(NewLocation, EndLocation) <= 10.0f)
		{
			Destroy();
		}
    }
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}