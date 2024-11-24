// Fill out your copyright notice in the Description page of Project Settings.


#include "Environnement/Terrain.h"


#define GRID_WIDTH 15
#define GRID_HEIGHT 15

static char DefaultGrid[GRID_HEIGHT][GRID_WIDTH];

// Sets default values
ATerrain::ATerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GridWidth = GRID_WIDTH;
	GridHeight = GRID_HEIGHT;


}

int32 ATerrain::GetGridWidth() const
{
	return GridWidth;
}

int32 ATerrain::GetGridHeight() const
{
	return GridHeight;
}

const TArray<TArray<AActor*>>& ATerrain::GetTerrainGrid() const
{
	return TerrainGrid;
}

void ATerrain::CreateTerrain(int32 X, int32 Y)
{
	if (X >= GridWidth || Y >= GridHeight) {
		return;
	}

	FRotator QuarterTurn[4] = {
		FRotator::ZeroRotator,
		FRotator(0, 90, 0),
		FRotator(0, 180, 0),
		FRotator(0, 270, 0)
	};

	if (TerrainGrid[Y][X] == nullptr) {
		FVector SpawnLocation = FVector(X * 200.0f, Y * 200.0f, 0.0f);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* NewTerrain = GetWorld()->SpawnActor<AActor>(TerrainClass[DefaultGrid[X][Y]], SpawnLocation, QuarterTurn[FMath::RandRange(0, 3)], SpawnParams);
		if (NewTerrain) {
			NewTerrain->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			TerrainGrid[Y][X] = NewTerrain;
		}
	}
}

bool ATerrain::VerifyPossibleCreate() const
{
	// Obtenez le nombre de classes dans TerrainClass
	int32 NumberOfClasses = TerrainClass.Num();

	// Parcourez chaque élément de DefaultGrid
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			// Vérifiez si l'élément est dans la plage du nombre de classes
			if (DefaultGrid[Y][X] >= NumberOfClasses)
			{
				// Si l'élément n'est pas dans la plage, retournez une erreur
				UE_LOG(LogTemp, Error, TEXT("Element at (%d, %d) in DefaultGrid is out of range."), X, Y);
				return false;
			}
		}
	}
	return true;
}

// Called when the game starts or when spawned
void ATerrain::BeginPlay()
{
	Super::BeginPlay();

	//initialise grid
	TerrainGrid.SetNum(GridHeight);
	for (auto& Row : TerrainGrid) {
		Row.SetNum(GridWidth);
	}

	//initialise default grid with random values between 0 and TerrainClass.Num()
	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			//DefaultGrid[Y][X] = FMath::RandRange(0, TerrainClass.Num() - 1);
			DefaultGrid[Y][X] = 0;
		}
	}

	// Create terrain
	DefaultInitialization();


}

void ATerrain::DefaultInitialization()
{
	for (int Y = 0; Y < GridHeight; ++Y) {
		for (int X = 0; X < GridWidth; ++X) {
			CreateTerrain(X, Y);
		}
	}
}


