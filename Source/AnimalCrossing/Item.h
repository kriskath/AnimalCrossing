// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UStaticMeshComponent;


UENUM()
enum class EItemType : uint8
{
	Equipable UMETA(DisplayName = "Equipable"), // Player menu inv
	Consumable UMETA(DisplayName = "Consumable"), // Player menu inv
	Material UMETA(DisplayName = "Material"), // Player menu inv
};

UCLASS()
class ANIMALCROSSING_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// To determine if object is in scene or not
	bool GetActive();

	// Used to disable item in scene after interacted with
	void Touched();

	UPROPERTY(EditAnywhere, Category = "Item") // EditAnywhere allows for changing the item in the details panel
		FString Name;

	UPROPERTY(EditAnywhere, Category = "Item")
		int Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		EItemType ItemType; // Item Type: Equipables, Consumables, Materials (Should never be Storage)
private:

	UPROPERTY(VisibleAnywhere, Category = "Item") // VisibleAnywhere so it is only editable before play in details panel
		class UStaticMeshComponent* ItemMesh;
	UPROPERTY()
		bool isVisible;

};
