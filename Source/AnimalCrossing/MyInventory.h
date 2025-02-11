// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyInventory.generated.h"


UENUM()
enum class EInventoryType : uint8
{
	Equipable UMETA(DisplayName = "Equipables"), // Player menu inv
	Consumable UMETA(DisplayName = "Consumables"), // Player menu inv
	Material UMETA(DisplayName = "Materials"), // Player menu inv
	Storage UMETA(DisplayName = "Storage") // Chest inv for every type of item
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANIMALCROSSING_API UMyInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyInventory();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Fills inventory with empty values
	void InitializeInventory();
	// To iterate through inventory
	void Iterate(float value);
	// Adds item in inventory
	void AddItem(FString Name, int Amount);


	UPROPERTY(BlueprintReadOnly)
		TArray<FString> ItemNames; // Item names

	UPROPERTY(BlueprintReadOnly)
		TArray<int> ItemAmounts; // Item amounts

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
		EInventoryType InventoryType; // Type of inventory storage : Equipables, Consumables, Materials, Storage

private:
	UPROPERTY(EditAnywhere, Category = "Inventory")
	int InventorySize; // Size of inventory

	UPROPERTY()
	int SelectedI; // For current selected inventory slot
};
