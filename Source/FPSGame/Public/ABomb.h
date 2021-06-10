// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABomb.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class UMaterialInstanceDynamic;

UCLASS()
class FPSGAME_API AABomb : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
		float ExplodeDelay;
	// Sets default values for this actor's properties
	AABomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	float TimeAlive;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* MeshComp;
	UFUNCTION()
		void Explode();
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
		UParticleSystem* ExplosionTemplate;
	UMaterialInstanceDynamic* MaterialInstance;
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
	FLinearColor CurrentColor;
	UPROPERTY(EditDefaultsOnly, Category = "Bombs")
	FLinearColor TargetColor;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
