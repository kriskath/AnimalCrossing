// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewInventoryWidget.generated.h"


class UTextBlock;
class UUniformGridPanel;
/**
 * 
 */
UCLASS(Blueprintable, Abstract)
class ANIMALCROSSING_API UNewInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere, Category = "Inventory Panel")
		FText LabelText;

	UPROPERTY(EditAnywhere, Category = "Inventory Panel")
		TSubclassOf<UUserWidget> ItemWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category = "Inventory Panel")
		int32 Columns = 4;

	UPROPERTY(EditAnywhere, Category = "Inventory Panel")
		int32 Rows = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Panel",
		meta = (BindWidget))
		UTextBlock* Label = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Inventory Panel",
		meta = (BindWidget))
		UUniformGridPanel* Grid = nullptr;
};
