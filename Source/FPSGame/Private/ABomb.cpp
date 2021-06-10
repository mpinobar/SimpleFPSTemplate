// Fill out your copyright notice in the Description page of Project Settings.


#include "ABomb.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Engine/Engine.h"
#include <string>
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

using namespace std;

// Sets default values
AABomb::AABomb()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplodeDelay = 2.0f;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AABomb::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle explodeTimerHandle;
	GetWorldTimerManager().SetTimer(explodeTimerHandle, this, &AABomb::Explode, ExplodeDelay);

	MaterialInstance = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

	if (MaterialInstance) {
		CurrentColor = MaterialInstance->K2_GetVectorParameterValue("Color");
	}
	TargetColor = FLinearColor::MakeRandomColor();
	TimeAlive = 0;
}

// Called every frame
void AABomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TimeAlive += DeltaTime / ExplodeDelay;
	if (TimeAlive > 1)
		TimeAlive = 1;

	if (MaterialInstance) {
		FLinearColor NextColor = FLinearColor::LerpUsingHSV(CurrentColor, TargetColor, TimeAlive);
		if (GEngine) {

			GEngine->AddOnScreenDebugMessage(0, 15.0f, FColor::Blue, *FString::FromInt(TimeAlive * 100));
		}
		MaterialInstance->SetVectorParameterValue("Color", NextColor);
	}

}
void AABomb::Explode() {

	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionTemplate, GetActorLocation());
	TArray<FOverlapResult> OutOverlaps;

	FCollisionObjectQueryParams queryParams;
	queryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	queryParams.AddObjectTypesToQuery(ECC_PhysicsBody);

	FCollisionShape collisionShape;
	collisionShape.SetSphere(500);
	
	GetWorld()->OverlapMultiByObjectType(OutOverlaps, GetActorLocation(), FQuat::Identity, queryParams, collisionShape);

	for (FOverlapResult result : OutOverlaps)
	{
		UPrimitiveComponent* overlap = result.GetComponent();
		if (overlap && overlap->IsSimulatingPhysics()) {
			float ExplosionForce = 50;
			overlap->AddImpulse(overlap->GetRelativeLocation() * ExplosionForce);
			UMaterialInstanceDynamic* MatInst = overlap->CreateAndSetMaterialInstanceDynamic(0);
			MatInst->SetVectorParameterValue("Color", FLinearColor::MakeRandomColor());
		}
	}

	Destroy();
}
