// Copyright Ben Tristem 2016

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Runtime/Engine/Classes/Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();

	if (!pressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *GetOwner()->GetName());
	}
	// ...
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetTotalMassOfActorsOnPlate() > triggerMass) {
		OnOpen.Broadcast();
	}
	else {
		OnClose.Broadcast();
	}
	// ...
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
	float totalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!pressurePlate) return totalMass;
	pressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* actor : OverlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}
