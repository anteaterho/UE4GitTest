// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4GitTest.h"
#include "MyHUD.h"
#include "Engine.h"

void AMyHUD::PostRender()
{
	const ULocalPlayer *LP = Cast<ULocalPlayer>(PlayerOwner->Player);
	FVector2D MousePos = LP->ViewportClient->GetMousePosition();

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
	Canvas->DrawText(SampleFont, SampleText, Pos.X, Pos.Y);


	FString TraceLocDebug = TEXT("[ Hit XYZ ]");
	FString TraceActorDebug = TEXT("[ Hit Actor ]");
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

		Canvas->DrawText(SampleFont, TraceLocDebug, Pos.X, Pos.Y + 20.f);
		
		if (TraceHitResult.GetActor() != NULL)
		{
			SampleFont->SetFontScalingFactor(10.f);
			TraceActorDebug = TraceActorDebug.operator+=(" ");
			TraceActorDebug = TraceActorDebug.operator+=(TraceHitResult.GetActor()->GetName());
			
			FVector TargetScreenPos = Canvas->Project(TraceEnd);

			//Canvas->DrawText(SampleFont, TraceActorDebug, Pos.X, Pos.Y + 40.0f);
			Canvas->DrawText(SampleFont, TraceActorDebug, TargetScreenPos.X + 10.f, TargetScreenPos.Y);
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



