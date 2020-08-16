// Copyright Epic Games, Inc. All Rights Reserved.

#include "AnimalCrossingCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Item.h"
#include "MyInventory.h"
#include "Components/SphereComponent.h"

//////////////////////////////////////////////////////////////////////////
// AAnimalCrossingCharacter

AAnimalCrossingCharacter::AAnimalCrossingCharacter()
{
	// Tells Unreal what character is currently selected
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Arm->SetupAttachment(RootComponent);
	Arm->TargetArmLength = 300.f;
	Arm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));

	Arm->bEnableCameraLag = true; // Enables camera lag for movement
	Arm->CameraLagSpeed = 2; // How quick camera moves
	Arm->CameraLagMaxDistance = 1.5f; // Distance of camera is maxed at

	Arm->bEnableCameraRotationLag = true;
	Arm->CameraRotationLagSpeed = 4;
	Arm->CameraLagMaxTimeStep = 1;

	Cam->SetupAttachment(Arm, USpringArmComponent::SocketName); // Attaches camera to end of arm

	Jumping = false;

	FirstPerson = false;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	//bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;


	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);




	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	// Sets Range of pickup for character
	Range = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionRange"));
	Range->SetupAttachment(RootComponent);
	Range->SetSphereRadius(100.0f);

	canInteract = false;
	IsInteracting = false;
	InteractionType = "null";
	AddType = "null";
	AddAmount = 0;
	Counter = 0;
	Delay = 0;

	NearByInteraction = "null";
}

void AAnimalCrossingCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Searches for characters component of class UMyInventory and sets reference to Inventory pointer
	Inventory = this->FindComponentByClass<UMyInventory>();
}

void AAnimalCrossingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Jumping)
	{
		Jump();
	}

	if (IsInteracting)
	{
		Counter += DeltaTime;
		if (Counter >= Delay)
		{
			CollectFromArea();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAnimalCrossingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up gameplay key bindings
	check(PlayerInputComponent);

	InputComponent->BindAxis("Horizontal", this, &AAnimalCrossingCharacter::HorizontalMove);
	InputComponent->BindAxis("Vertical", this, &AAnimalCrossingCharacter::VerticalMove);
	InputComponent->BindAxis("HorizontalRot", this, &AAnimalCrossingCharacter::HorizontalRot);
	InputComponent->BindAxis("VerticalRot", this, &AAnimalCrossingCharacter::VerticalRot);
	InputComponent->BindAxis("Zoom", this, &AAnimalCrossingCharacter::Zoom);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AAnimalCrossingCharacter::CheckJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AAnimalCrossingCharacter::CheckJump);
	InputComponent->BindAction("ChangeCamera", IE_Pressed, this, &AAnimalCrossingCharacter::Switch);


	InputComponent->BindAction("Interacting", IE_Pressed, this, &AAnimalCrossingCharacter::Interact);
	InputComponent->BindAction("Pickup", IE_Pressed, this, &AAnimalCrossingCharacter::CallAdd);
	InputComponent->BindAxis("Iterate", this, &AAnimalCrossingCharacter::CallIterate);

}


void AAnimalCrossingCharacter::CheckJump()
{
	if (Jumping)
	{
		Jumping = false;
	}
	else
	{
		Jumping = true;
	}
}

void AAnimalCrossingCharacter::VerticalRot(float value)
{
	if (value)
	{
		float temp = 0;
		if (FirstPerson)
		{
			temp = Cam->GetRelativeRotation().Pitch + value;
			if (temp < 65 && temp > -65)
			{
				Cam->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
		else 
		{
			temp = Arm->GetRelativeRotation().Pitch + value;
			if (temp < 25 && temp > -65)
			{
				Arm->AddLocalRotation(FRotator(value, 0, 0));
			}
		}
	}
}
void AAnimalCrossingCharacter::HorizontalRot(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}
void AAnimalCrossingCharacter::HorizontalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AAnimalCrossingCharacter::VerticalMove(float value)
{
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AAnimalCrossingCharacter::Zoom(float value)
{
	if (value)
	{
		float temp = Arm->TargetArmLength + (value * -10);
		if (temp < 310 && temp > 140)
		{
			Arm->TargetArmLength = temp;
		}
	}
}

void AAnimalCrossingCharacter::Switch()
{
	if (FirstPerson)
	{
		Arm->TargetArmLength = 300.f;
		Arm->SetRelativeRotation(FRotator(-45, 0, 0));
		Cam->SetRelativeRotation(FRotator(0, 0, 0));
		Cam->SetupAttachment(Arm, USpringArmComponent::SocketName);
		FirstPerson = false;
	}
	else
	{
		FAttachmentTransformRules rules;
		Cam->AttachToComponent(RootComponent, rules, );
		Cam->SetupAttachment(RootComponent);
		FirstPerson = true;
	}
}



void AAnimalCrossingCharacter::CanInteract(bool Able, FString Area, FString Item, int Amount)
{
	canInteract = Able;
	InteractionType = Area;
	AddType = Item;
	AddAmount = Amount;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Can Interact"));
	NearByInteraction = Area;
}

void AAnimalCrossingCharacter::Interact()
{
	if (InteractionType != "null" && canInteract && !IsInteracting)
	{
		NearByInteraction = "null";
		IsInteracting = true;
		Counter = 0;
		Delay = 3;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Is Interacting"));
	}
	else if (InteractionType != "null" && canInteract && IsInteracting)
	{
		IsInteracting = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exited Interact"));
	}
}

void AAnimalCrossingCharacter::CollectFromArea()
{
	if (Inventory)
	{
		if (InteractionType != "null")
		{
			Inventory->AddItem(AddType, AddAmount);
			IsInteracting = false;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Finished Interaction"));
		}
	}
}


void AAnimalCrossingCharacter::CallIterate(float value)
{
	// Check if value and Inventory aren't null
	if (value && Inventory)
	{
		// Call iterate function
		Inventory->Iterate(value);
	}
}


void AAnimalCrossingCharacter::CallAdd()
{
	if (Inventory)
	{
		TArray<AActor*> CollectionItems;
		Range->GetOverlappingActors(CollectionItems);

		for (int i = 0; i < CollectionItems.Num(); i++)
		{
			// Attempt to cast item overlapping with range. If cast succeeds then it is pickupable, else its just part of scene
				// If cast fails then tempItem will be NULL
			AItem* const tempItem = Cast<AItem>(CollectionItems[i]);

			// Check if it exsits, if is not already being picked up, and if active in scene
			if (tempItem && !tempItem->IsPendingKill() && tempItem->GetActive())
			{
				Inventory->AddItem(tempItem->Name, tempItem->Amount);
				tempItem->Touched();
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, tempItem->Name);
			}
		}
	}
}
