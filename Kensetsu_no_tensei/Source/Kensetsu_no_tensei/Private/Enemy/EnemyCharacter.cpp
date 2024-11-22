// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Road/RoadPath.h"
#include "Components/SplineComponent.h"
#include "Chateau/Chateau.h"
#include "Kismet/GameplayStatics.h"

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

		//Update accumulated distance
		accumDistance += Speed * DeltaTime;

		//Get new location and rotation along spline
		FVector NewLocation = PathToFollow->SplineComponent->GetLocationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
		SetActorLocation(NewLocation + FVector(0, 0, 150));
		FRotator SplineRotation = PathToFollow->SplineComponent->GetRotationAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
		SetActorRotation(SplineRotation);

		//Slow down fast enemies in corners
		if (isFastEnemy)
		{
			//Get current and next frame tangent
			FVector NextTangent = PathToFollow->SplineComponent->GetTangentAtDistanceAlongSpline(accumDistance + Speed * DeltaTime, ESplineCoordinateSpace::World);
			FVector CurrentTangent = PathToFollow->SplineComponent->GetTangentAtDistanceAlongSpline(accumDistance, ESplineCoordinateSpace::World);
			
			float angle = FMath::Acos(FVector::DotProduct(CurrentTangent.GetSafeNormal(), NextTangent.GetSafeNormal()));
			angle = FMath::RadiansToDegrees(angle);
			float slowingFactor = 1.0f - FMath::Clamp(angle / 90.0f, 0.0f, 0.25f); //remove maximum 1/4 of current speed each frame
			if (angle <= 1.0f)
			{
				//Progressively reset speed in straight lines
				if (Speed >= 200.0f)
				{
					Speed = 200.0f;
				}
				else
				{
					Speed += Speed * slowingFactor;
				}
			}
			else
			{
				//Slow down enemy
				Speed *= slowingFactor;
			}
		}

		//Destroy and update castle's life if at end of the road
		if (FVector::Dist(NewLocation, EndLocation) <= 10.0f)
		{
			Destroy();
			AChateau* Chateau = Cast<AChateau>(UGameplayStatics::GetActorOfClass(GetWorld(), AChateau::StaticClass()));
			Chateau->ModifyLife(-AmountDamage);
		}
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}