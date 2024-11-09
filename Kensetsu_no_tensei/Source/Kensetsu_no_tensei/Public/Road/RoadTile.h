// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadTile.generated.h"

UCLASS()
class KENSETSU_NO_TENSEI_API ARoadTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadTile();

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class UMaterialInstanceDynamic* DynamicMaterialInstance;

	bool Accessible = true;

	void SetColor(const FLinearColor& NewColor);

	UPROPERTY(EditAnywhere, Category = "Road Mesh");
	TObjectPtr<class UStaticMesh> RoadMesh0;

	UPROPERTY(EditAnywhere, Category = "Road Mesh");
	TObjectPtr<class UStaticMesh> RoadMesh1;

	UPROPERTY(EditAnywhere, Category = "Road Mesh")
	TObjectPtr<class UStaticMesh> RoadMesh2Line;

	UPROPERTY(EditAnywhere, Category = "Road Mesh")
	TObjectPtr<class UStaticMesh> RoadMesh2Curv;

	UPROPERTY(EditAnywhere, Category = "Road Mesh")
	TObjectPtr<class UStaticMesh> RoadMesh3;

	UPROPERTY(EditAnywhere, Category = "Road Mesh")
	TObjectPtr<class UStaticMesh> RoadMesh4;

	void SetMesh(int32 BitCount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
