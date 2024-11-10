// Fill out your copyright notice in the Description page of Project Settings.


#include "Chateau/Chateau.h"
#include "Components/BoxComponent.h"

// Sets default values
AChateau::AChateau()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create BoxComponent and set as RootComponent for the Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	// Create StaticMeshComponent and Attach to BoxComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	maxLife = 100;

}

// Called when the game starts or when spawned
void AChateau::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChateau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChateau::ModifyLife(float Delta) {
	Life += Delta;
}

float AChateau::GetLife() {
	return Life;
}