// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadManager.h"
#include "Road/RoadTile.h"
#include "Engine/World.h"

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

static char DefaultGrid[GRID_HEIGHT][GRID_WIDTH] =
{
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 1, 0, 0, 1, 0, 0,
	0, 0, 1, 0, 1, 1, 1, 1, 0, 0,
	1, 1, 1, 1, 1, 0, 1, 0, 0, 0,
	0, 0, 1, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
	0, 0, 1, 1, 1, 1, 1, 0, 1, 1,
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0



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
	GridWidth = 10;
	GridHeight = 10;
}

void ARoadManager::CreateRoadTile(int32 X, int32 Y)
{
	if (X >= GridWidth || Y >= GridHeight) {
		return;
	}

	if (RoadGrid[Y][X] == nullptr) {
		FVector SpawnLocation = FVector(X * 200.0f, Y * 200.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		RoadGrid[Y][X] = GetWorld()->SpawnActor<ARoadTile>(RoadTileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		RoadGrid[Y][X]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ARoadManager::DestroyRoadTile(int32 X, int32 Y)
{
	if (X >= GridWidth || Y >= GridHeight) {
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
	GridWidth = 10;
	GridHeight = 10;
	Super::BeginPlay();
	
	RoadGrid.SetNum(GridHeight);
	for (auto& Row : RoadGrid) {
		Row.SetNum(GridWidth);
	}

	DefaultInitialization(); // Just here for tests, to remove later

	{ // Spawn both end points
		FActorSpawnParameters SpawnParams;

		FVector SpawnLocation = FVector(4 * 200.0f, -1 * 200.0f, 0.0f);
		ARoadTile* TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		SpawnLocation = FVector(4 * 200.0f, GridHeight * 200.0f, 0.0f);
		TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ARoadManager::DefaultInitialization()
{
	for (int Y = 0; Y < GRID_HEIGHT; ++Y) {
		for (int X = 0; X < GRID_WIDTH; ++X) {
			if (DefaultGrid[Y][X]) {
				CreateRoadTile(X, Y);
			}
		}
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




