// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventory.h"

// Sets default values for this component's properties
UMyInventory::UMyInventory()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	InventorySize = 8;
	SelectedI = 0;

	InventoryType = EInventoryType::Material;
	// ...
}


// Called when the game starts
void UMyInventory::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeInventory();
}


// Called every frame
void UMyInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyInventory::InitializeInventory()
{
	for (int i = 0; i < InventorySize; i++)
	{
		ItemNames.Add("Empty");
		ItemAmounts.Add(0);
	}
}

void UMyInventory::Iterate(float value)
{
	SelectedI += value;
	if (SelectedI < 0)
	{
		SelectedI = InventorySize - 1;
	}
	else if (SelectedI >= InventorySize)
	{
		SelectedI = 0;
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, ItemNames[SelectedI]);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::FromInt(ItemAmounts[SelectedI]));
}

void UMyInventory::AddItem(FString Name, int Amount)
{
	int FirstEmpty = -1; // To check if there is an empty slot, if still -1 then no empty slots
	bool Has = false; // To check if item already exists in inventory

	for (int i = 0; i < InventorySize; i++)
	{
		// Case where item exists
		if (ItemNames[i] == Name)
		{
			ItemAmounts[i] += Amount;
			Has = true;
			break;
		}
		// Case where first empty slot exists
		else if (ItemNames[i] == "Empty" && FirstEmpty == -1)
		{
			FirstEmpty = i;
		}
	}
	// If it doesn't exist then check for first empty slot 
	if (!Has)
	{
		// If first empty slot exists then add it there
		if (FirstEmpty != -1)
		{
			ItemNames[FirstEmpty] = Name;
			ItemAmounts[FirstEmpty] = Amount;
		}
		// Inventory is full and cannot add item
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Inventory Full!"));
		}
	}
}

