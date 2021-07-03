#pragma once

#include "CoreMinimal.h"
#include "Subsystem/ModSubsystem.h"
#include "FGCharacterPlayer.h"
#include "Components/WidgetInteractionComponent.h"

#include "PTSDInteractionSubsystem.generated.h"

UCLASS()
class PTSDINTERACTION_API APTSDInteractionSubsystem final : public AModSubsystem {
	GENERATED_BODY()
	
public:
	APTSDInteractionSubsystem();

	// Begin AActor Overrides
	virtual void BeginPlay() override;
	virtual void Tick(float dt) override;
	// End AActor Overrides

	// Begin Interaction Events
	void OnPrimaryFirePressed();
	void OnPrimaryFireReleased();
	void OnSecondaryFirePressed();
	void OnSecondaryFireReleased();
	// End Interaction Events
	
	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction", meta = (WorldContext = "WorldContext"))
	static APTSDInteractionSubsystem* GetInteractionSubsystem(UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction")
	void AttachWidgetInteractionToPlayer(AFGCharacterPlayer* Character);
	
	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction")
	void DetachWidgetInteractionToPlayer(AFGCharacterPlayer* Character);

	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction")
	void EnableTestForKBFocus();
	
	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction")
	void RegisterDesiresKBFocusWidget(UWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "PTSD | Interaction")
	void UnregisterDesiresKBFocusWidget(UWidget* Widget);
	
private:
	int VirtualUserNum = 0;

	UPROPERTY()
	TMap<AFGCharacterPlayer*, UWidgetInteractionComponent*> WidgetInteractionComponents;

	UPROPERTY()
	UInputComponent* Input;

	UPROPERTY()
	TArray<UWidget*> KBFocusTestWidgets;

	UPROPERTY()
	bool TestForKBFocus = false;
};