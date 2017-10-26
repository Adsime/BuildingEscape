// Copyright Ben Tristem 2016

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!physicsHandler) return;

	if (physicsHandler->GrabbedComponent) {
		physicsHandler->SetTargetLocation(GetLineTraceEnd());
	}
	

	// ...
}

void UGrabber::Grab() {
	auto hitResult = GetFirstPhysicsObjectInReach();
	auto componentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();
	if(actorHit) {
		if (!physicsHandler) return;
		physicsHandler->GrabComponentAtLocation(componentToGrab, NAME_None, componentToGrab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::Release() {
	if (!physicsHandler) return;
	physicsHandler->ReleaseComponent();
}

FVector UGrabber::GetLineTraceStart() {
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->
		GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation;
}

FVector UGrabber::GetLineTraceEnd() {
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->
		GetPlayerViewPoint(OUT playerViewPointLocation, OUT playerViewPointRotation);
	return playerViewPointLocation + (playerViewPointRotation.Vector() * reach);
}

void UGrabber::FindPhysicsComponent() {
	physicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandler) {
		UE_LOG(LogTemp, Warning, TEXT("Unable to find physics handler"));
	}
}

void UGrabber::SetupInputComponent() {
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent) {
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *(GetOwner()->GetName()));
	}
}

const FHitResult UGrabber::GetFirstPhysicsObjectInReach() {
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	FHitResult hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetLineTraceStart(),
		GetLineTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
		);
	return hit;
}



