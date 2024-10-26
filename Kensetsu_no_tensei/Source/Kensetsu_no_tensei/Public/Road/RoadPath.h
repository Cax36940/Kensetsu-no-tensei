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

	UPROPERTY(EditAnywhere, Category = "Components")
	class USplineMeshComponent* SplineMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeSplinePoints();

	ARoadManager* FindRoadManager();



};
