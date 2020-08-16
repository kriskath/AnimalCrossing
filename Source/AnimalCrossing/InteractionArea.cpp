// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionArea.h"
#include "GameFramework/Actor.h"
#include "AnimalCrossingCharacter.h"


AInteractionArea::AInteractionArea()
{
	OnActorBeginOverlap.AddDynamic(this, &AInteractionArea::EnterArea);
	OnActorEndOverlap.AddDynamic(this, &AInteractionArea::ExitArea);
}

void AInteractionArea::BeginPlay()
{
	Super::BeginPlay();
}

void AInteractionArea::EnterArea(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AAnimalCrossingCharacter* character = Cast<AAnimalCrossingCharacter>(otherActor);
		if (character)
		{
			character->CanInteract(true, AreaType, AreaItem, AreaAmount);
		}
	}
}


void AInteractionArea::ExitArea(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AAnimalCrossingCharacter* character = Cast<AAnimalCrossingCharacter>(otherActor);
		if (character)
		{
			character->CanInteract(false, "null", "null", 0);
		}
	}
}