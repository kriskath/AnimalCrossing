// Fill out your copyright notice in the Description page of Project Settings.


#include "ExampleWidget.h"
#include "Components/TextBlock.h"

UExampleWidget::UExampleWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UExampleWidget::NativeConstruct()
{
	// Do some custom setup

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

	/*
		Even if you don't mark the widget as optional in your property tags, 
		you can still run your game with a Blueprint subclass that has not defined a matching widget. 
		So you should generally do nullptr checks.
	*/
	if (TitleBlock)
	{
		TitleBlock->SetText(FText::FromString("Hello World"));
	}
	if (SubtitleBlock)
	{
		SubtitleBlock->SetText(FText::FromString("Hello World"));
	}
}

void UExampleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here
}