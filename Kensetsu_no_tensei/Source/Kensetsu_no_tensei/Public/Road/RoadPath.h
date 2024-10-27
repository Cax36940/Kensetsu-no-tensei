// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadPath.generated.h"

class ARoadManager;

UCLASS()
class KENSETSU_NO_TENSEI_API ARoadPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadPath();

	UPROPERTY(EditAnywhere, Category = "Components")
	class USplineComponent* SplineComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	TSubclassOf<AActor> Marker;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeSplinePoints();

	ARoadManager* FindRoadManager();

	void AddPointToSpline(int32 X, int32 Y, const FVector& Offset);

};
