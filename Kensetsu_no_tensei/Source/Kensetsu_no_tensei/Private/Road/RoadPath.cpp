// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadPath.h"
#include "Road/RoadManager.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "EngineUtils.h"

// Sets default values
ARoadPath::ARoadPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SplineComponent and set as RootComponent for the Actor
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	RootComponent = SplineComponent;

	// Create StaticMeshComponent and Attach to BoxComponent
	SplineMeshComponent = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMeshComponent"));
	SplineMeshComponent->SetupAttachment(SplineComponent);

}

// Called when the game starts or when spawned
void ARoadPath::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSplinePoints();

}

// Called every frame
void ARoadPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoadPath::InitializeSplinePoints()
{
	ARoadManager* RoadManager = FindRoadManager();
	if (!RoadManager) {
		UE_LOG(LogTemp, Warning, TEXT("RoadManager not found for Spline initialization"));
		return;
	}

	SplineComponent->ClearSplinePoints();

	SplineComponent->AddSplinePoint(FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	SplineComponent->AddSplinePoint(FVector(500, 0, 0), ESplineCoordinateSpace::Local);

	SplineComponent->UpdateSpline();
}

ARoadManager* ARoadPath::FindRoadManager()
{
	for (TActorIterator<ARoadManager> It(GetWorld()); It; ++It) {
		return *It;
	}
	return nullptr;
}

