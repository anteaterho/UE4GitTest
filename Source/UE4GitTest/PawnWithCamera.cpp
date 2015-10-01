// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4GitTest.h"
#include "PawnWithCamera.h"
#include "DrawDebugHelpers.h"

// Sets default values
APawnWithCamera::APawnWithCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	OurCameraSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	OurCameraSpringArm->AttachTo(RootComponent);
	OurCameraSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	OurCameraSpringArm->TargetArmLength = 40.0f;
	OurCameraSpringArm->bEnableCameraLag = true;
	OurCameraSpringArm->CameraLagSpeed = 3.0f;
	OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	OurCamera->AttachTo(OurCameraSpringArm, USpringArmComponent::SocketName);

	Count = 0;
	Distance = 100.0f;
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void APawnWithCamera::BeginPlay()
{
	Super::BeginPlay();

	//Print Actor name.
	FString MyName = GetName();
	//UE_LOG(LogClass, Warning, TEXT("This is a %s"), *MyName);
}


// Called every frame
void APawnWithCamera::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	FVector testLoc = GetActorLocation();
	UE_LOG(LogClass, Warning, TEXT("Test LOC %s"), *testLoc.ToString());	

	if (bZoomingin)
	{
		ZoomFactor += DeltaTime / 0.5f;
	}
	else
	{
		ZoomFactor -= DeltaTime / 0.25f;
	}

	ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);
	OurCamera->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
	OurCameraSpringArm->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);

	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += CameraInput.X;
		SetActorRotation(NewRotation);
	}

	{
		FRotator NewRotation = OurCameraSpringArm->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		OurCameraSpringArm->SetWorldRotation(NewRotation);
	}

	{
		if (!MovementInput.IsZero())
		{
			//Scale our movement input axis values by 100 units per second
			MovementInput = MovementInput.SafeNormal() * 100.0f;
			FVector NewLocation = GetActorLocation();
			
			//Print Actor Location.
			//UE_LOG(LogClass, Error	, TEXT("This is a testing statement. %s"), *NewLocation.ToString());
			//UE_LOG(LogClass, Warning, TEXT("Delta Time %f"), DeltaTime);

			NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
			NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
			this->SetActorLocation(NewLocation);
		}
	}

	//FHitResult HitCall(ForceInit);
	//FCollisionQueryParams ParamCall = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	//DoTrace(&HitCall, &ParamCall);

	Test();
}

// Called to bind functionality to input
void APawnWithCamera::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &APawnWithCamera::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &APawnWithCamera::ZoomOut);

	InputComponent->BindAxis("MoveForward", this, &APawnWithCamera::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APawnWithCamera::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &APawnWithCamera::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APawnWithCamera::YawCamera);
}


void APawnWithCamera::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.0f, 1.0f);
}

void APawnWithCamera::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void APawnWithCamera::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void APawnWithCamera::ZoomIn()
{
	bZoomingin = true;
}

void APawnWithCamera::ZoomOut()
{
	bZoomingin = false;
}

bool APawnWithCamera::DoTrace(FHitResult* Hit, FCollisionQueryParams* Params)
{

	Count += 1;
	UE_LOG(LogClass, Warning, TEXT("Count : %d"), Count);

	FVector Loc = CameraOne->GetActorLocation();
	UE_LOG(LogClass, Error, TEXT("Cameera Loc is %S"), *Loc.ToString());
	FRotator Rot = CameraOne->GetActorRotation();
	UE_LOG(LogClass, Warning, TEXT("Cameera Rot is %S"), *Rot.ToString());

	/*
	FVector Loc = GetActorLocation();
	FRotator Rot = GetActorRotation();
	*/

	FVector Start = Loc;
	FVector End = Loc + (Rot.Vector() * 100.0f);
	UE_LOG(LogClass, Warning, TEXT("Cameera End is %S"), *End.ToString());

	Params->bTraceComplex = true;
	Params->bTraceAsyncScene = true;
	Params->bReturnPhysicalMaterial = true;
	bool Traced = GetWorld()->LineTraceSingle(*Hit, Start, End, ECC_PhysicsBody, *Params);
	return Traced;
}

void APawnWithCamera::Test()
{
	FVector Loc = CameraOne->GetActorLocation();
	UE_LOG(LogClass, Error, TEXT("Loc is %s"), *Loc.ToString());
	FRotator Rot = CameraOne->GetActorRotation();
	UE_LOG(LogClass, Error, TEXT("Rot is %s"), *Rot.ToString());
	FVector Start = Loc;
	UE_LOG(LogClass, Error, TEXT("Start is %s"), *Start.ToString());
	FVector End = Loc + (Rot.Vector() * Distance);
	UE_LOG(LogClass, Error, TEXT("End is %s"), *End.ToString());
	TempActor->SetActorLocation(End);

	FCollisionQueryParams TraceParam = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	TraceParam.bTraceComplex = true;
	TraceParam.bTraceAsyncScene = true;
	TraceParam.bReturnPhysicalMaterial = false;
	TraceParam.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);

	GetWorld()->LineTraceSingle(Hit, Start, End, ECC_Pawn, TraceParam);
	DrawDebugLine(GetWorld(), Start, End, FColor(255, 0, 0), false, -1, 0, 12.33f);

	if (Hit.bBlockingHit)
	{
		UE_LOG(LogClass, Error, TEXT("Hit Something"));
	}
	else
	{
		UE_LOG(LogClass, Error, TEXT("No Hit"));
	}
}
