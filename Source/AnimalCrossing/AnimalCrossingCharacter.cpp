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
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Sets Range of pickup for character
	Range = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionRange"));
	Range->AttachTo(RootComponent);
	Range->SetSphereRadius(100.0f);
}

void AAnimalCrossingCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Searches for characters component of class UMyInventory and sets reference to Inventory pointer
	Inventory = this->FindComponentByClass<UMyInventory>();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAnimalCrossingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{

	// Set up gameplay key bindings
	check(PlayerInputComponent);


	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AAnimalCrossingCharacter::CallAdd);
	PlayerInputComponent->BindAxis("Iterate", this, &AAnimalCrossingCharacter::CallIterate);



	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAnimalCrossingCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAnimalCrossingCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAnimalCrossingCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAnimalCrossingCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAnimalCrossingCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAnimalCrossingCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAnimalCrossingCharacter::OnResetVR);
}


void AAnimalCrossingCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAnimalCrossingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AAnimalCrossingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AAnimalCrossingCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAnimalCrossingCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAnimalCrossingCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAnimalCrossingCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
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
