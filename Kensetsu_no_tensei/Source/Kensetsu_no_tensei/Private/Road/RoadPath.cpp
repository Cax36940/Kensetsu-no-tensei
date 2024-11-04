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

static int32 Heuristic(int32 X, int32 Y, int32 endX, int32 endY) {
	return abs(X - endX) + abs(Y - endY);
}

// Insert in log(N) while preserving decreasing order
static void InsertInOrder(TArray<FAStarNode>& Queue, const FAStarNode& Element) {
	int Left = 0;
	int Right = Queue.Num();
	
	while (Left < Right) {
		int Middle = (Right + Left) / 2;
		if (Queue[Middle].HValue > Element.HValue) {
			Left = Middle + 1;
		}
		else {
			Right = Middle;
		}
	}

	Queue.Insert(Element, Left);
}

// Return true if end is reachable from (X, Y) while not using Visited tiles
static bool CanReachEnd(int32 X, int32 Y, int32 endX, int32 endY, const TArray<TArray<ARoadTile*>>& Grid, TArray<TArray<char>> Visited) {
	const int32 Width = Grid[0].Num();
	const int32 Height = Grid.Num();

	TArray<FAStarNode> PriorityQueue;
	PriorityQueue.Emplace(Heuristic(X, Y, endX, endY), 0, X, Y);

	const char Direction[4][2] = 
	   { 1,  0,
		-1,  0, 
		 0,  1, 
		 0, -1};

	while (!PriorityQueue.IsEmpty()) {
		FAStarNode CurrentNode = PriorityQueue.Pop();
		if (CurrentNode.X == endX && CurrentNode.Y == endY) {
			return true;
		}

		Visited[CurrentNode.Y][CurrentNode.X] = 1;
		for (int i = 0; i < 4; ++i) {
			const int32 NewX = CurrentNode.X + Direction[i][0];
			const int32 NewY = CurrentNode.Y + Direction[i][1];

			if (0 <= NewX && NewX < Width && 0 <= NewY && NewY < Height &&
				Grid[NewY][NewX] != nullptr && Visited[NewY][NewX] == 0) {
				InsertInOrder(PriorityQueue, FAStarNode(CurrentNode.Dist + 1 + Heuristic(NewX, NewY, endX, endY), CurrentNode.Dist + 1, NewX, NewY));
			}
		}
	}

	return false;
}

void ARoadPath::InitializeSplinePoints() {

	// Get RoadManager
	ARoadManager* RoadManager = FindRoadManager();
	if (!RoadManager) {
		UE_LOG(LogTemp, Warning, TEXT("RoadManager not found for Spline initialization"));
		return;
	}

	const TArray<TArray<ARoadTile*>>& ConstRoadGrid = RoadManager->GetRoadGrid();

	TArray<TArray<char>> Visited;
	const int32 Width = RoadManager->GetGridWidth();
	const int32 Height = RoadManager->GetGridHeight();

	Visited.SetNum(Height);
	for (auto& Row : Visited) {
		Row.SetNum(Width);
	}

	for (int Y = 0; Y < Height; Y++) {
		for (int X = 0; X < Width; X++) {
			Visited[Y][X] = 0;
		}
	}

	int32 CurrentX = Width / 2 - 1;
	int32 CurrentY = Height - 1;

	const int32 EndX = Width / 2 - 1;
	const int32 EndY = 0;

	const char Direction[4][2] =
	{ 1,  0,
	 -1,  0,
	  0,  1,
	  0, -1 };

	SplineComponent->ClearSplinePoints();

	AddPointToSpline(CurrentX, CurrentY + 1, RoadManager->GetActorLocation());

	while (true) {
		AddPointToSpline(CurrentX, CurrentY, RoadManager->GetActorLocation());
		if (CurrentX == EndX && CurrentY == EndY) {
			break;
		}
		Visited[CurrentY][CurrentX] = 1;

		char DirIndex[4] = {0, 1, 2, 3};
		// Shuffle Direction
		for (int i = 3; i > 0; --i) {
			std::swap(DirIndex[i], DirIndex[std::rand() % (i + 1)]);
		}

		for (int i = 0; i < 4; ++i) {
			const int32 NewX = CurrentX + Direction[DirIndex[i]][0];
			const int32 NewY = CurrentY + Direction[DirIndex[i]][1];

			if (0 <= NewX && NewX < Width && 0 <= NewY && NewY < Height &&
				ConstRoadGrid[NewY][NewX] != nullptr && Visited[NewY][NewX] == 0 && CanReachEnd(NewX, NewY, EndX, EndY, ConstRoadGrid, Visited)) {
				CurrentX = NewX;
				CurrentY = NewY;
				break;
			}
		}
	}
	
	AddPointToSpline(CurrentX, CurrentY - 1, RoadManager->GetActorLocation());

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

