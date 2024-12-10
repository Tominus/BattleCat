#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BC_Player.generated.h"

class ABC_Unit;
class ABC_FightHUD;
class ABC_UnitBase;


/* Le Player stock toutes les variables et fonctionnalitées relatives à l'argent, le spawn d'Unit, et les event d'UI */

UCLASS()
class BATTLECAT_API ABC_Player : public APawn
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSpawned, ABC_Unit*, _unit);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpgradeComplete);

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnUnitSpawned onUnitSpawned;

	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnUpgradeComplete onUpgradeComplete;

private:
	// Variable utilisée lors du spawn des Units pour éviter la superposition de plusieurs rendu.
	UPROPERTY(EditAnywhere, Category = "Player|Settings", meta = (ClampMin = "1.0", ClampMax = "30.0"))
	float spawnDeepOffset = 30.0f;

	// Stock tous les BP des Unit que le player possède. (Sert pour l'affichage en UI et le spawn d'Unit)
	UPROPERTY(EditAnywhere, Category = "Player|Money") TArray<TSubclassOf<ABC_Unit>> allUnits;

	// Les données relatifs à l'argent.
	UPROPERTY(VisibleAnywhere, Category = "Player|Money") float currentMoney = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Player|Money") float maxMoney = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Player|Money") float earnMoneySpeed = 1.5f;
	
	// Les données relatifs aux améliorations du WorkerCat.
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") float addMaxMoney = 40.0f;
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") float addEarnMoneySpeed = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") int workerCatUpgradeCost = 40;
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") int workerCatUpgradeCostToAdd = 40;
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") int workerCatCurrentLevel = 1;
	UPROPERTY(EditAnywhere, Category = "Player|WorkerCat") int workerCatMaxLevel = 8;


	UPROPERTY()	ABC_FightHUD* hud = nullptr;
	UPROPERTY()	ABC_UnitBase* unitBase = nullptr;

public:
	FORCEINLINE void SetUnitBase(ABC_UnitBase* _unitBase) { unitBase = _unitBase; }
	
public:
	ABC_Player();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	void InitPlayer();

	// Donne toutes les données des Units à l'HUD avec un delay le temps que l'HUD se setup.
	UFUNCTION() void InitPlayerHUD();

	// Update l'argent gagné par le joueur et le rendu de l'UI.
	void UpdateMoney(float _deltaTime);

	// Améliore les stats d'argent du joueur si il peut et update le rendu d'UI.
	UFUNCTION() void UpgradeWorkerCat();

	// Calcul quelle unitée doit être spawn et son coût.
	UFUNCTION() void SpawnCat(int _catIndex);

public:
	void AddPlayerMoney(int _amount);
	
};