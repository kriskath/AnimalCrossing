// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExampleWidget.generated.h"


class UTextBlock;

// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS(Abstract)
class ANIMALCROSSING_API UExampleWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UExampleWidget(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// An in engine error will display if item is not created with BindWidget
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TitleBlock = nullptr;


	// No error shown if the Blueprint class does not have a widget with that name with BindWidgetOptional
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* SubtitleBlock = nullptr;

};
