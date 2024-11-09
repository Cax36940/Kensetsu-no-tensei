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

void ARoadTile::SetMesh(int32 BitCount)
{

	int32 MeshIndex = 0;

	for (int i = 1; i < 0b10000; i <<= 1) {
		if (i & BitCount) {
			MeshIndex++;
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 5.0, FColor::Red, FString::Printf(TEXT("%d"), MeshIndex));

	switch (MeshIndex) {
	case 0:
		if (RoadMesh0) {
			StaticMesh->SetStaticMesh(RoadMesh0);
		}
		break;

	case 1:
		if (RoadMesh1) {
			StaticMesh->SetStaticMesh(RoadMesh1);

		switch (BitCount) {
		case 0b0100:
			SetActorRotation(FRotator(0, 90, 0));
			break;

		case 0b0010:
			SetActorRotation(FRotator(0, 180, 0));
			break;

		case 0b0001:
			SetActorRotation(FRotator(0, 270, 0));
			break;

		default: // 0b1000
			SetActorRotation(FRotator(0, 0, 0));
			break;
		}

		}
		break;



	case 2:
		if (BitCount == 0b1010 || BitCount == 0b0101) {
			if (RoadMesh2Line) {
				StaticMesh->SetStaticMesh(RoadMesh2Line);

				if (BitCount == 0b1010) {
					SetActorRotation(FRotator(0, 0, 0));
				}
				else {
					SetActorRotation(FRotator(0, 90, 0));
				}

			}
		}
		else if (RoadMesh2Curv) {
			StaticMesh->SetStaticMesh(RoadMesh2Curv);
			switch (BitCount) {
			case 0b1100:
				SetActorRotation(FRotator(0, 90, 0));
				break;

			case 0b0110:
				SetActorRotation(FRotator(0, 180, 0));
				break;

			case 0b0011:
				SetActorRotation(FRotator(0, 270, 0));
				break;

			default: // 0b1001
				SetActorRotation(FRotator(0, 0, 0));
				break;
			}
		}
		break;

	case 3:
		if (RoadMesh3) {
			StaticMesh->SetStaticMesh(RoadMesh3);

			switch (BitCount) {
				case 0b0111 :
					SetActorRotation(FRotator(0, 270, 0));
					break;

				case 0b1011:
					SetActorRotation(FRotator(0, 0, 0));
					break;

				case 0b1101:
					SetActorRotation(FRotator(0, 90, 0));
					break;

				default : // 0b1110
					SetActorRotation(FRotator(0, 180, 0));
					break;
			}

		}
		break;

	case 4:
		if (RoadMesh4) {
			StaticMesh->SetStaticMesh(RoadMesh4);
		}
		break;
	
	default:
		break;
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

