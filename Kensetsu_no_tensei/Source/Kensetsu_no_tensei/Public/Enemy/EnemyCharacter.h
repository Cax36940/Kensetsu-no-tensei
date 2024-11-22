// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

public:
	// Sets default values for this pawn's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ARoadPath* PathToFollow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float accumDistance = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, Category = "Road Management")
	TSubclassOf<ARoadPath> RoadPathClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AmountDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isFastEnemy = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
