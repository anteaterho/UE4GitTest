// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4GitTest.h"
#include "RayCasting.h"


// Sets default values
ARayCasting::ARayCasting()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	Count = 0;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ARayCasting::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARayCasting::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/*
	APawn* CheckPawn = FindPawnCameraIsLookingAt();
	if (CheckPawn != nullptr)
	{
		FString PawnName = CheckPawn->GetName();
		UE_LOG(LogClass, Warning, TEXT("Check Pawn : %s"), *PawnName);
	}
	*/

	FVector Loc2 = GetActorLocation();
	FRotator Rot2 = GetActorRotation();
	UE_LOG(LogClass, Error, TEXT("Rot is: %S"), *Rot2.ToString());

	Count += 1;
	UE_LOG(LogClass, Warning, TEXT("Count : %d"), Count);

	//FHitResult HitCall(ForceInit);
	//FCollisionQueryParams ParamCall = FCollisionQueryParams(FName(TEXT("Trace")), true, this);
	//DoTrace(&HitCall, &ParamCall);
}

// Called to bind functionality to input
void ARayCasting::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

bool ARayCasting::DoTrace(FHitResult* Hit, FCollisionQueryParams* Params)
{	
	
	FVector Loc = this->GetActorLocation();
	UE_LOG(LogClass, Warning, TEXT("Cameera Loc is %S"), *Loc.ToString());
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

