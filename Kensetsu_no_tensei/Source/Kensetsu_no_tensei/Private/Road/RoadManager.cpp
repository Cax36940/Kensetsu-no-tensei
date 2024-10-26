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
	RoadGrid.SetNum(GRID_WIDTH);
	for (auto& Row : RoadGrid) {
		Row.SetNum(GRID_HEIGHT);
	}
}

void ARoadManager::CreateRoadTile(int32 X, int32 Y)
{
	if (X >= GRID_WIDTH || Y >= GRID_HEIGHT) {
		return;
	}

	if (RoadGrid[X][Y] == nullptr) {
		FVector SpawnLocation = FVector(X * 200.0f, Y * 200.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		RoadGrid[X][Y] = GetWorld()->SpawnActor<ARoadTile>(RoadTileClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		RoadGrid[X][Y]->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void ARoadManager::DestroyRoadTile(int32 X, int32 Y)
{
	if (X >= GRID_WIDTH || Y >= GRID_HEIGHT) {
		return;
	}

	if (RoadGrid[X][Y]) {
		RoadGrid[X][Y]->Destroy();
		RoadGrid[X][Y] = nullptr;
	}
}

// Called when the game starts or when spawned
void ARoadManager::BeginPlay()
{
	Super::BeginPlay();
	
	{ // Spawn both end points
		FActorSpawnParameters SpawnParams;

		FVector SpawnLocation = FVector(4 * 200.0f, -1 * 200.0f, 0.0f);
		ARoadTile* TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

		SpawnLocation = FVector(4 * 200.0f, GRID_HEIGHT * 200.0f, 0.0f);
		TmpActor = GetWorld()->SpawnActor<ARoadTile>(RoadEndClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
		TmpActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}

	for (int i = 0; i < GRID_HEIGHT; ++i) {
		for (int j = 0; j < GRID_WIDTH; ++j) {
			if (DefaultGrid[i][j]) {
				CreateRoadTile(j, i);
			}
		}
	}
	
}

// Called every frame
void ARoadManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



