// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "InteractionArea.generated.h"


class AActor;

/**
 * 
 */
UCLASS()
class ANIMALCROSSING_API AInteractionArea : public ATriggerBox
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

public:
	AInteractionArea();

	// Activates when actor enters area
	UFUNCTION()
		void EnterArea(AActor* overlappedActor, AActor* otherActor);

	// Deactivates when actor leaves area
	UFUNCTION()
		void ExitArea(AActor* overlappedActor, AActor* otherActor);


	UPROPERTY(EditAnywhere)
		FString AreaType;
	UPROPERTY(EditAnywhere)
		FString AreaItem;
	UPROPERTY(EditAnywhere)
		int AreaAmount;


};
