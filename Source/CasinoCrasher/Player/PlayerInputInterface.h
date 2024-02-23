#pragma once

#include "PlayerInputInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInputInterface : public UInterface
{
	GENERATED_BODY()
};

class CASINOCRASHER_API IPlayerInputInterface
{
	GENERATED_BODY()

public:

	/** Move */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Move(const struct FInputActionValue& Value);
	virtual void Move_Implementation(const struct FInputActionValue& Value) {};

	/** Look */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Look(const FInputActionValue& Value);
	virtual void Look_Implementation(const struct FInputActionValue& Value) {};
	
	/** Jump */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Jump();
	virtual void Jump_Implementation() {};

	/** Start Sprint */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StartSprint();
	virtual void StartSprint_Implementation() {};

	/** Stop Sprint */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopSprint();
	virtual void StopSprint_Implementation() {};

	/** Roll */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Roll();
	virtual void Roll_Implementation() {};

	/** Attack 1 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Attack1();
	virtual void Attack1_Implementation() {};
	
	/** Attack 2 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Attack2();
	virtual void Attack2_Implementation() {};
	
	/** Attack 3 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Attack3();
	virtual void Attack3_Implementation() {};
	
	/** Interact */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Interact();
	virtual void Interact_Implementation() {};

	/** Pause */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void Pause();
	virtual void Pause_Implementation() {};
};
