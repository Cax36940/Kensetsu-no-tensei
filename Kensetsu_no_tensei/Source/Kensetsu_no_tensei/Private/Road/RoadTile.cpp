// Fill out your copyright notice in the Description page of Project Settings.


#include "Road/RoadTile.h"
#include "Components/BoxComponent.h"

// Sets default values
ARoadTile::ARoadTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BoxComponent and set as RootComponent for the Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	// Create StaticMeshComponent and Attach to BoxComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	DynamicMaterialInstance = CreateDefaultSubobject<UMaterialInstanceDynamic>(TEXT("DynamicMaterial"));

}

void ARoadTile::SetColor(const FLinearColor& NewColor)
{
	if (DynamicMaterialInstance) {
		DynamicMaterialInstance->SetVectorParameterValue("Color", NewColor);
	}
}

// Called when the game starts or when spawned
void ARoadTile::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMesh) {
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(StaticMesh->GetMaterial(0), this, TEXT("DynamicMaterial"));
		if (DynamicMaterialInstance) {
			StaticMesh->SetMaterial(0, DynamicMaterialInstance);
		}
	}
	
}

// Called every frame
void ARoadTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

