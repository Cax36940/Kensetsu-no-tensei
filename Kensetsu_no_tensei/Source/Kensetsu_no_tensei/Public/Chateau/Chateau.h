// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chateau.generated.h"

UCLASS()
class KENSETSU_NO_TENSEI_API AChateau : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;




	
public:	
	// Sets default values for this actor's properties
	AChateau();
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float GetLife();
	

	void ModifyLife(float);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float Life;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxLife;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
