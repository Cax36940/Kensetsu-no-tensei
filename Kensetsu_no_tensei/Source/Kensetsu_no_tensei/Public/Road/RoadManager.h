// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadManager.generated.h"

class ARoadTile;

UCLASS()
class KENSETSU_NO_TENSEI_API ARoadManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadManager();

	UFUNCTION(BlueprintCallable, Category = "Road Management")
	void CreateRoadTile(int32 X, int32 Y);

	UFUNCTION(BlueprintCallable, Category = "Road Management")
	void DestroyRoadTile(int32 X, int32 Y);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	int32 GridWidth;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	int32 GridHeight;

	TArray<TArray<ARoadTile*>> RoadGrid;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	TSubclassOf<ARoadTile> RoadTileClass;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	TSubclassOf<ARoadTile> RoadEndClass;

	void DefaultInitialization();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetGridWidth() const;

	int32 GetGridHeight() const;

	const TArray<TArray<ARoadTile*>>& GetRoadGrid() const;

};
