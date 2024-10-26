// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Road/RoadPath.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class KENSETSU_NO_TENSEI_API AEnemyCharacter : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	class ARoadPath* RoadPath;

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Money;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
