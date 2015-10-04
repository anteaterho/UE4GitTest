// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE4GITTEST_API AMyHUD : public AHUD
{
	GENERATED_BODY()

		virtual void PostRender() override;
	
public:

	FVector GetMOuseWorldPosition();
	
private:
	FVector MouseWorldPosition;

};
