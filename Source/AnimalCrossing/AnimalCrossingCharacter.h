// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AnimalCrossingCharacter.generated.h"

class USphereComponent;
class UMyInventory;

UCLASS(config=Game)
class AAnimalCrossingCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AAnimalCrossingCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	// Determines what kind of collection area player is in
	void CanInteract(bool Able, FString Area, FString Item, int Amount);


	UPROPERTY(BlueprintReadOnly)
		FString NearByInteraction;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
		UMyInventory* MaterialsInventory;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
		UMyInventory* ConsumablesInventory;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
		UMyInventory* EquipablesInventory;
private:

	// Determines if player interacts with area or to cut action short
	void Interact();

	// Collects items from area into inventory
	void CollectFromArea();

	// Calls inventory iterate function
	void CallIterate(float value);

	// Figures out when to add items to inventory
	void CallAdd();


	UPROPERTY()
		bool canInteract; // When player is inside trigger box
	UPROPERTY()
		bool IsInteracting; // When player interacts for animations
	UPROPERTY()
		FString InteractionType; // Holds type of value for area
	UPROPERTY()
		FString AddType; // Holds value of item passed through interaction
	UPROPERTY()
		int AddAmount; // Holds amount value
	UPROPERTY()
		float Counter; // Tracks amount of time since interaction
	UPROPERTY()
		float Delay; // Tracks if enough time has passed
	UPROPERTY()
		USphereComponent* Range; // Interaction range on character
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);


protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	void HorizontalMove(float value);
	void VerticalMove(float value);
	void HorizontalRot(float value);
	void VerticalRot(float value);

	UPROPERTY()
		USpringArmComponent* Arm;

	void CheckJump();

	UPROPERTY()
		bool Jumping;

	void Zoom(float value);

	UPROPERTY()
		UCameraComponent* Cam;
};

