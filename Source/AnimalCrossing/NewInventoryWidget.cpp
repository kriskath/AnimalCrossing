// Fill out your copyright notice in the Description page of Project Settings.


#include "NewInventoryWidget.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

// This is called every time that the widget is compiled,
// or a property is changed.
void UNewInventoryWidget::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	// When first creating a Blueprint subclass of this class,
	// the widgets won't exist, so we must null check.
	if (Label)
	{
		Label->SetText(LabelText);
	}


	// Again, null checks are required
	if (Grid && ItemWidgetClass)
	{
		Grid->ClearChildren();

		for (int32 y = 0; y < Rows; ++y)
		{
			for (int32 x = 0; x < Columns; ++x)
			{
				UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), ItemWidgetClass);
				if (Widget)
				{
					UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(Widget);
					GridSlot->SetColumn(x);
					GridSlot->SetRow(y);
				}
			}
		}
	}
}