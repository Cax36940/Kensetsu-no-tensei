// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadManager.h"
#include "Road/RoadTile.h"
#include "Road/RoadPath.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "HAL/PlatformTime.h"

#define GRID_WIDTH 15
#define GRID_HEIGHT 15

static char DefaultGrid[GRID_HEIGHT][GRID_WIDTH] =
{
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0



	/* To reset path
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	*/
};

// Sets default values
ARoadManager::ARoadManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GridWidth = GRID_WIDTH;
	GridHeight = GRID_HEIGHT;
}

void ARoadManager::CreateRoadTile(int32 X, int32 Y)
{

	if (X >= GridWidth || X < 0 || Y >= GridHeight || Y < 0) {
		return;
	}

	if (RoadGrid[Y][X] == nullptr) {
		FVector SpawnLocation = FVector(X * 200.0f, Y * 200.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		RoadGrid[Y][X] = GetWorld()->SpawnActor<ARoadTile>(RoadTileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		RoadGrid[Y][X]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		UpdateRoadMeshAround(X, Y);
		UpdateRoadColor();
	}
}
void ARoadManager::DestroyRoadTile(int32 X, int32 Y)
{
	if (X >= GridWidth || X < 0 || Y >= GridHeight || Y < 0) {
		return;
	}

	if (RoadGrid[Y][X]) {
		RoadGrid[Y][X]->Destroy();
		RoadGrid[Y][X] = nullptr;
	}
}

// Called when the game starts or when spawned
void ARoadManager::BeginPlay()
{
	GridWidth = GRID_HEIGHT;
	GridHeight = GRID_HEIGHT;
	Super::BeginPlay();
	
	RoadGrid.SetNum(GridHeight);
	for (auto& Row : RoadGrid) {
		Row.SetNum(GridWidth);
	}

	//DefaultInitialization(); // Just here for tests, to remove later
	UpdateRoadColor();
	UpdateRoadMeshAll();
	/* { // Spawn both end points
		FActorSpawnParameters SpawnParams;

		FVector SpawnLocation = FVector(4 * 200.0f, -1 * 200.0f, 0.0f);
		ARoadTile* TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		SpawnLocation = FVector(4 * 200.0f, GridHeight * 200.0f, 0.0f);
		TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}*/
}

void ARoadManager::DefaultInitialization()
{
	for (int Y = 0; Y < GRID_HEIGHT; ++Y) {
		for (int X = 0; X < GRID_WIDTH; ++X) {
			if (DefaultGrid[Y][X]) {
				CreateRoadTile(X, Y);
			}
			else {
				//CreateClickableTower(X, Y);
			}
		}
	}
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
	  0, -1 };

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

void ARoadManager::UpdateRoadColor()
{
	FLinearColor RedColor(0.4f, 0.4f, 0.4f, 0.8f);
	FLinearColor GreenColor(1.0f, 1.0f, 1.0f, 1.0f);

	TArray<TArray<char>> Visited;

	Visited.SetNum(GridHeight);
	for (auto& Row : Visited) {
		Row.SetNum(GridWidth);
	}

	for (int Y = 0; Y < GRID_HEIGHT; ++Y) {
		for (int X = 0; X < GRID_WIDTH; ++X) {
			if (RoadGrid[Y][X]) {
				RoadGrid[Y][X]->SetColor(RedColor);
				Visited[Y][X] = 0;
			}
		}
	}

	const int32 BeginX = GridWidth / 2;
	const int32 BeginY = GridHeight - 1;

	const int32 EndX = GridWidth / 2;
	const int32 EndY = 0;

	if (!CanReachEnd(BeginX, BeginY, EndX, EndY, RoadGrid, Visited)) {
		canReachEnd = false;
		return;
	}
	canReachEnd = true;

	TArray<TPair<int32, int32>> CoordList;
	int32 count = 0;

	while (count < 100) {
		CreatePath(CoordList);
		for (const auto& coord : CoordList) {
			if (0 <= coord.Key && coord.Key < GRID_WIDTH && 0 <= coord.Value && coord.Value < GRID_HEIGHT && Visited[coord.Value][coord.Key] == 0) {
				RoadGrid[coord.Value][coord.Key]->SetColor(GreenColor);
				Visited[coord.Value][coord.Key] = 1;
				count = 0;
			}
		}
		count++;
	}

}

void ARoadManager::UpdateRoadMeshAll()
{
	for (int Y = 0; Y < GRID_HEIGHT; ++Y) {
		for (int X = 0; X < GRID_WIDTH; ++X) {
			UpdateRoadMesh(X, Y);
		}
	}
}

void ARoadManager::UpdateRoadMeshAround(int32 X, int32 Y)
{

	UpdateRoadMesh(X, Y);

	const char Direction[4][2] =
	{ 1,  0,
	  0,  1,
	 -1,  0,
	  0, -1 };

	for (int i = 0; i < 4; ++i) {
		const int32 NewX = X + Direction[i][0];
		const int32 NewY = Y + Direction[i][1];

		if (0 <= NewX && NewX < GridWidth && 0 <= NewY && NewY < GridHeight &&
			RoadGrid[NewY][NewX] != nullptr) {
			UpdateRoadMesh(NewX, NewY);
		}
	}

}

void ARoadManager::UpdateRoadMesh(int32 X, int32 Y)
{
	const char Direction[4][2] =
	{ 1,  0,
	  0,  1,
	 -1,  0,
	  0, -1 };

	if (RoadGrid[Y][X]) {

		int32 bitCount = 0;

		for (int i = 0; i < 4; ++i) {
			const int32 NewX = X + Direction[i][0];
			const int32 NewY = Y + Direction[i][1];

			bitCount <<= 1;
			if (0 <= NewX && NewX < GridWidth && 0 <= NewY && NewY < GridHeight &&
				RoadGrid[NewY][NewX] != nullptr) {
				bitCount |= 0b1;
			}
		}

		if (X == GridWidth / 2 && Y == GridHeight - 1) {
			bitCount |= 0b0100;
		}
		else if (X == GridWidth / 2 && Y == 0) {
					bitCount |= 0b0001;
		}

		RoadGrid[Y][X]->SetMesh(bitCount);
	}

}

int32 ARoadManager::GetGridWidth() const
{
	return GridWidth;
}

int32 ARoadManager::GetGridHeight() const
{
	return GridHeight;
}

const TArray<TArray<ARoadTile*>>& ARoadManager::GetRoadGrid() const
{
	return RoadGrid;
}

// Called every frame
void ARoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ARoadManager::CreatePath(TArray<TPair<int32, int32>>& PathVec) {

	PathVec.Empty();

	TArray<TArray<char>> Visited;

	Visited.SetNum(GridHeight);
	for (auto& Row : Visited) {
		Row.SetNum(GridWidth);
	}

	for (int Y = 0; Y < GridHeight; Y++) {
		for (int X = 0; X < GridWidth; X++) {
			Visited[Y][X] = 0;
		}
	}

	int32 CurrentX = GridWidth / 2;
	int32 CurrentY = GridHeight - 1;

	const int32 EndX = GridWidth / 2;
	const int32 EndY = 0;

	const char Direction[4][2] =
	{ 1,  0,
	 -1,  0,
	  0,  1,
	  0, -1 };


	PathVec.Emplace(CurrentX, CurrentY + 1);

	while (true) {
		PathVec.Emplace(CurrentX, CurrentY);
		if (CurrentX == EndX && CurrentY == EndY) {
			break;
		}
		Visited[CurrentY][CurrentX] = 1;

		char DirIndex[4] = { 0, 1, 2, 3 };
		// Shuffle Direction
		for (int i = 3; i > 0; --i) {
			std::swap(DirIndex[i], DirIndex[std::rand() % (i + 1)]);
		}

		for (int i = 0; i < 4; ++i) {
			const int32 NewX = CurrentX + Direction[DirIndex[i]][0];
			const int32 NewY = CurrentY + Direction[DirIndex[i]][1];

			if (0 <= NewX && NewX < GridWidth && 0 <= NewY && NewY < GridHeight &&
				RoadGrid[NewY][NewX] != nullptr && Visited[NewY][NewX] == 0 && CanReachEnd(NewX, NewY, EndX, EndY, RoadGrid, Visited)) {
				CurrentX = NewX;
				CurrentY = NewY;
				break;
			}
		}
	}

	PathVec.Emplace(CurrentX, CurrentY - 1);

}



