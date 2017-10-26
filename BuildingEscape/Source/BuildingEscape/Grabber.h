// Copyright Ben Tristem 2016

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float reach = 100.f;
	UPhysicsHandleComponent* physicsHandler = nullptr;
	UInputComponent* inputComponent = nullptr;
	void FindPhysicsComponent();
	void SetupInputComponent();
	const FHitResult GetFirstPhysicsObjectInReach();
	void Grab();
	void Release();
	FVector GetLineTraceStart();
	FVector GetLineTraceEnd();
};
