// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4GitTest.h"
#include "MyHUD.h"
#include "PawnWithCamera.h"
#include "Engine.h"

void AMyHUD::PostRender()
{
	const ULocalPlayer *LP = Cast<ULocalPlayer>(PlayerOwner->Player);
	FVector2D MousePos = LP->ViewportClient->GetMousePosition();

	//APawnWithCamera *MyPawn = Cast<APawnWithCamera>(UGameplayStatics::GetPlayerPawn(this, 0)->GetPawn);
	//float dist = MyPawn->DistanceWithToPoints;
	//float dist = 0.f;

	//Get Player Pawn(PawnWithCamera)
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	APawnWithCamera *MyPawn = Cast<APawnWithCamera>(MyController->GetPawn());
	float dist = MyPawn->GetDistanceWithToPoints();
	bool bShowLog = MyPawn->GetShowLog();

	UFont *SampleFont = GEngine->GetLargeFont();
	FVector2D Pos;

	float XL, YL;
	FString SampleText = TEXT("[ Screen XY ]");
	SampleText = SampleText.operator+=(FString::SanitizeFloat(MousePos.X));
	SampleText = SampleText.operator+=(" ");
	SampleText = SampleText.operator+=(FString::SanitizeFloat(MousePos.Y));

	Canvas->StrLen(SampleFont, SampleText, XL, YL);

	Pos.X = (Canvas->SizeX / 2) - XL / 2;
	Pos.Y = (Canvas->SizeY / 2);

	
	Canvas->SetDrawColor(FColor(20, 255, 10, 255));
	Canvas->DrawText(SampleFont, SampleText, Pos.X, Pos.Y - 30.f);


	FString TraceLocDebug = TEXT("[ Hit XYZ ]");
	FString TraceActorDebug = TEXT("[ Hit Actor ]");
	FString TraceDistance = TEXT("[ Hit Distance ]");
	FVector WorldOrigin, WorldDirection, TraceEnd;
	FHitResult TraceHitResult;
	FCollisionObjectQueryParams TraceObjectParam;
	FCollisionQueryParams TraceParam;

	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	TraceObjectParam.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	TraceParam.bTraceComplex = true;
	TraceParam.bTraceAsyncScene = true;
	TraceParam.bReturnPhysicalMaterial = false;


	if (Canvas != NULL && PlayerOwner != NULL)
	{
		//Screen to world
		Canvas->Deproject(MousePos, WorldOrigin, WorldDirection);
		TraceEnd = WorldOrigin + WorldDirection * 6536.0f;

		GetWorld()->LineTraceSingle(TraceHitResult, WorldOrigin, TraceEnd, TraceParam, TraceObjectParam);
		MouseWorldPosition = TraceHitResult.ImpactPoint;

		TraceLocDebug = TraceLocDebug.operator+=(" ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.X)));
		TraceLocDebug = TraceLocDebug.operator+=(" , ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.Y)));
		TraceLocDebug = TraceLocDebug.operator+=(" , ");
		TraceLocDebug = TraceLocDebug.operator+=(FString::FromInt(int(MouseWorldPosition.Z)));

		Canvas->DrawText(SampleFont, TraceLocDebug, Pos.X, Pos.Y - 10.f);
		
		if (TraceHitResult.GetActor() != NULL && bShowLog == true)
		{
			SampleFont->SetFontScalingFactor(10.f);
			TraceActorDebug = TraceActorDebug.operator+=(" ");
			TraceActorDebug = TraceActorDebug.operator+=(TraceHitResult.GetActor()->GetName());
			
			//world to screen
			FVector TargetScreenPos = Canvas->Project(TraceEnd);
			FVector2D MiddlePoint = Pos + ((FVector2D::FVector2D(TargetScreenPos) - Pos) * 0.5f);

			//Canvas->DrawText(SampleFont, TraceActorDebug, Pos.X, Pos.Y + 40.0f);
			Canvas->DrawText(SampleFont, TraceActorDebug, TargetScreenPos.X + 10.f, TargetScreenPos.Y);

			TraceDistance = TraceDistance.operator+=(" ");
			TraceDistance = TraceDistance.operator+=(FString::FromInt(int(dist)));
			Canvas->DrawText(SampleFont, TraceDistance, MiddlePoint.X, MiddlePoint.Y);

		}
	}
	else if (Canvas == NULL || PlayerOwner == NULL)
	{
		MouseWorldPosition = FVector(0.f, 0.f, 0.f);
		TraceLocDebug = TraceLocDebug.operator+=(TEXT("Canvas or PC NONE"));
	}
}

FVector AMyHUD::GetMOuseWorldPosition()
{
	return MouseWorldPosition;
}



