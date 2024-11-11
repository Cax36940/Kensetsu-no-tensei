// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

UCLASS()
class KENSETSU_NO_TENSEI_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerrain();

	int32 GetGridWidth() const;

	int32 GetGridHeight() const;

	const TArray<TArray<AActor*>>& GetTerrainGrid() const;


	UFUNCTION(BlueprintCallable, Category = "Terrain Management")
	void CreateTerrain(int32 X, int32 Y);
	bool VerifyPossibleCreate() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Terrain Management")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, Category = "Terrain Management")
	int32 GridHeight;

	TArray<TArray<AActor*>> TerrainGrid;

	UPROPERTY(EditAnywhere, Category = "Terrain Management")
	TArray<TSubclassOf<AActor>> TerrainClass;



	void DefaultInitialization();


};

