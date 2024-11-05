// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadPath.h"
#include "Road/RoadManager.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include <vector>

// Sets default values
ARoadPath::ARoadPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create SplineComponent and set as RootComponent for the Actor
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
	RootComponent = SplineComponent;

}

// Called when the game starts or when spawned
void ARoadPath::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSplinePoints();

}

void ARoadPath::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	for (AActor* Actor : Markers) {
		Actor->Destroy();
	}
	Markers.Empty();
}

// Called every frame
void ARoadPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoadPath::InitializeSplinePoints() {

	// Get RoadManager
	ARoadManager* RoadManager = FindRoadManager();
	if (!RoadManager) {
		UE_LOG(LogTemp, Warning, TEXT("RoadManager not found for Spline initialization"));
		return;
	}

	SplineComponent->ClearSplinePoints();

	TArray<TPair<int32, int32>> CoordList;
	RoadManager->CreatePath(CoordList);

	for (const auto& coord : CoordList) {
		AddPointToSpline(coord.Key, coord.Value, RoadManager->GetActorLocation());
	}

	SplineComponent->UpdateSpline();
}


ARoadManager* ARoadPath::FindRoadManager()
{
	for (TActorIterator<ARoadManager> It(GetWorld()); It; ++It) {
		return *It;
	}
	return nullptr;
}

void ARoadPath::AddPointToSpline(int32 X, int32 Y, const FVector& Offset)
{
	FVector SplinePointLocation = FVector(X * 200, Y * 200, 0) + Offset;
	SplineComponent->AddSplinePoint(SplinePointLocation, ESplineCoordinateSpace::World);
	FActorSpawnParameters SpawnParams;
	AActor* PointMarker = GetWorld()->SpawnActor<AActor>(Marker, SplinePointLocation, FRotator::ZeroRotator, SpawnParams);
	PointMarker->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	Markers.Add(PointMarker);
}

