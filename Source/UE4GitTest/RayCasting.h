// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "RayCasting.generated.h"

UCLASS()
class UE4GITTEST_API ARayCasting : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARayCasting();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere)
	ACameraActor* CameraOne;

	int Count;
	
	bool DoTrace(FHitResult* Hit, FCollisionQueryParams* Params);
};
