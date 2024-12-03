#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BC_FightHUD.generated.h"

class UBC_FightWidget;


/* Le FightHUD permet d'avoir l'interface de combat. (upgrade, argent, units à spawn)
 * Détient les évènements de l'UI pour tous ceux qui en ont besoin.
 * Ces events sont appelées par les Widgets eux même.
 * A aussi des liens pour update le rendu des Widgets.*/

UCLASS()
class BATTLECAT_API ABC_FightHUD : public AHUD
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgradeButtonPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCanonButtonPressed);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpawnCatButtonPressed, int, _buttonIndex);
	
private:
	UPROPERTY(EditAnywhere, Category = "FightHUD")
	TSubclassOf<UBC_FightWidget> fightWidgetToSpawn;

	
	UPROPERTY()	UBC_FightWidget* fightWidget = nullptr;
	UPROPERTY() APlayerController* playerController = nullptr;

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnUpgradeButtonPressed onUpgradeButtonPressed;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnCanonButtonPressed onCanonButtonPressed;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnSpawnCatButtonPressed onSpawnCatButtonPressed;

public:
	FORCEINLINE FOnUpgradeButtonPressed& OnUpgradeButtonPressed() { return onUpgradeButtonPressed; }
	FORCEINLINE FOnCanonButtonPressed& OnCanonButtonPressed() { return onCanonButtonPressed; }
	FORCEINLINE FOnSpawnCatButtonPressed& OnSpawnCatButtonPressed() { return onSpawnCatButtonPressed; }

private:
	virtual void BeginPlay() override;

	// Rajoute le widget de combat à l'interface.
	void InitHUDWidgets();
	
public:
	// Envoie les données au Widget de Fight.
	UFUNCTION() void InitSpawnCatButtons(const TArray<TSubclassOf<class ABC_Unit>>& _allUnits);
	UFUNCTION() void UpdatePlayerMoney(float _currentMoney, float _maxMoney, int _workerCatUpgradeMoney, const TArray<TSubclassOf<ABC_Unit>>& _allUnits);
	UFUNCTION() void UpdateWorkerCatPrice(int _workerCatLevel, int _workerCatMaxLevel, int _workerCatPrice);

	// Les fonctions qui detiennent les Broadcast des events appelés uniquement par les Widgets.
	UFUNCTION() void UpgradeButtonPressed();
	UFUNCTION() void CanonButtonPressed();
	UFUNCTION() void SpawnCatButtonPressed(int _buttonIndex);
	
};