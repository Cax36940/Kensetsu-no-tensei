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

// Called every frame
void ARoadPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

static int32 CountNeighbours(const TArray<TArray<char>>& RoadGrid, int32 X, int32 Y) {
	int32 count = 0;
	if (X > 0) {
		count += RoadGrid[Y][X - 1];
	}
	if (X < RoadGrid[0].Num() - 1) {
		count += RoadGrid[Y][X + 1];
	}
	if (Y > 0) {
		count += RoadGrid[Y - 1][X];
	}
	if (Y < RoadGrid.Num() - 1) {
		count += RoadGrid[Y + 1][X];
	}
	return count;
}

static void TrimRoadGrid(TArray<TArray<char>>& RoadGrid, int32 CurrentX, int32 CurrentY) {
	bool loop = true;
	while (loop) {
		loop = false;
		for (int Y = 0; Y < RoadGrid.Num(); ++Y) {
			for (int X = 0; X < RoadGrid[0].Num(); ++X) {
				if (RoadGrid[Y][X] == 1 && CountNeighbours(RoadGrid, X, Y) == 1 && (X != CurrentX || Y != CurrentY) && (X != int(RoadGrid[0].Num() / 2) - 1 || Y != 0)) {
					RoadGrid[Y][X] = 0;
					loop = true;
				}
			}
		}
	}

	int delY = RoadGrid.Num();
	for (int Y = CurrentY + 1; Y < RoadGrid.Num(); ++Y) {
		int count = 0;
		for (int X = 0; X < RoadGrid[0].Num(); ++X) {
			count += RoadGrid[Y][X];
		}
		if (count == 1) {
			delY = Y;
			break;
		}
	}

	for (; delY < RoadGrid.Num(); ++delY) {
		for (int X = 0; X < RoadGrid[0].Num(); ++X) {
			RoadGrid[delY][X] = 0;
		}
	}




}

static void MoveToRandomNeighbour(const TArray<TArray<char>>& RoadGrid, int32& CurrentX, int32& CurrentY) {
	
	int32 count = CountNeighbours(RoadGrid, CurrentX, CurrentY);

	int32 neighbourIndex = FMath::RandRange(0, count - 1);

	if (CurrentX > 0 && RoadGrid[CurrentY][CurrentX - 1] == 1) {
		if (neighbourIndex == 0) {
			--CurrentX;
			return;
		}
		--neighbourIndex;
	}
	if (CurrentX < RoadGrid[0].Num() - 1 && RoadGrid[CurrentY][CurrentX + 1] == 1) {
		if (neighbourIndex == 0) {
			++CurrentX;
			return;
		}
		--neighbourIndex;
	}
	if (CurrentY > 0 && RoadGrid[CurrentY - 1][CurrentX] == 1) {
		if (neighbourIndex == 0) {
			--CurrentY;
			return;
		}
		--neighbourIndex;
	}
	if (CurrentY < RoadGrid.Num() - 1 && RoadGrid[CurrentY + 1][CurrentX] == 1) {
		if (neighbourIndex == 0) {
			++CurrentY;
			return;
		}
	}
}

void ARoadPath::InitializeSplinePoints()
{
	ARoadManager* RoadManager = FindRoadManager();
	if (!RoadManager) {
		UE_LOG(LogTemp, Warning, TEXT("RoadManager not found for Spline initialization"));
		return;
	}

	const TArray<TArray<ARoadTile*>>& ConstRoadGrid = RoadManager->GetRoadGrid();

	// Grid of 0 and 1 : 0 no road and 1 if road usefull to count neighbours
	TArray<TArray<char>> RoadGrid;
	const int32 Width = RoadManager->GetGridWidth();
	const int32 Height = RoadManager->GetGridHeight();
	RoadGrid.SetNum(Height);
	for (auto& Row : RoadGrid) {
		Row.SetNum(Width);
	}

	for (int Y = 0; Y < Height; Y++) {
		for (int X = 0; X < Width; X++) {
			RoadGrid[Y][X] = (ConstRoadGrid[Y][X] == nullptr ? 0 : 1);
		}
	}

	SplineComponent->ClearSplinePoints();

	int32 CurrentX = int(Width / 2) - 1;
	int32 CurrentY = Height - 1;

	while(CurrentX != int(Width / 2) - 1 || CurrentY != 0) {
		TrimRoadGrid(RoadGrid, CurrentX, CurrentY);
		FVector SplinePointLocation = FVector(CurrentX * 200, CurrentY * 200, 0) + RoadManager->GetActorLocation();
		SplineComponent->AddSplinePoint(SplinePointLocation, ESplineCoordinateSpace::World);
		FActorSpawnParameters SpawnParams;
		AActor* PointMarker = GetWorld()->SpawnActor<AActor>(Marker, SplinePointLocation, FRotator::ZeroRotator, SpawnParams);
		PointMarker->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		RoadGrid[CurrentY][CurrentX] = 0;
		MoveToRandomNeighbour(RoadGrid, CurrentX, CurrentY);
	}
	TrimRoadGrid(RoadGrid, CurrentX, CurrentY);
	FVector SplinePointLocation = FVector(CurrentX * 200, CurrentY * 200, 0) + RoadManager->GetActorLocation();
	SplineComponent->AddSplinePoint(SplinePointLocation, ESplineCoordinateSpace::World);
	FActorSpawnParameters SpawnParams;
	AActor* PointMarker = GetWorld()->SpawnActor<AActor>(Marker, SplinePointLocation, FRotator::ZeroRotator, SpawnParams);
	PointMarker->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	RoadGrid[CurrentY][CurrentX] = 0;
	MoveToRandomNeighbour(RoadGrid, CurrentX, CurrentY);


	SplineComponent->UpdateSpline();
}

ARoadManager* ARoadPath::FindRoadManager()
{
	for (TActorIterator<ARoadManager> It(GetWorld()); It; ++It) {
		return *It;
	}
	return nullptr;
}


