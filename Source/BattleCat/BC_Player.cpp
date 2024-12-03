#include "BC_Player.h"

#include "BC_FightGameMode.h"
#include "BC_FightHUD.h"
#include "BC_Unit.h"
#include "BC_UnitBase.h"


ABC_Player::ABC_Player()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABC_Player::BeginPlay()
{
	Super::BeginPlay();
	InitPlayer();
}

void ABC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateMoney(DeltaTime);
}

void ABC_Player::InitPlayer()
{
	ABC_FightGameMode* _gm = GetWorld()->GetAuthGameMode<ABC_FightGameMode>();
	if (!_gm)return;
	_gm->SetPlayer(this);
	
	APlayerController* _pc = GetWorld()->GetFirstPlayerController();
	if (!_pc) return;
	hud = _pc->GetHUD<ABC_FightHUD>();

	if (!hud)
	{
		UE_LOG(LogTemp, Error, TEXT("ABC_Player::InitPlayer-> Invalid hud"));
		return;
	}
	
	// Récupère les events du hud lors du click sur le bouton Upgrade ou sur les boutons de spawn d'Units.
	hud->OnUpgradeButtonPressed().AddUniqueDynamic(this, &ABC_Player::UpgradeWorkerCat);
	hud->OnSpawnCatButtonPressed().AddUniqueDynamic(this, &ABC_Player::SpawnCat);

	FTimerHandle _timerHandle;
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	_timerManager.SetTimer(_timerHandle, this, &ABC_Player::InitPlayerHUD, 0.01f);
}

void ABC_Player::InitPlayerHUD()
{
	hud->InitSpawnCatButtons(allUnits);
}

void ABC_Player::UpdateMoney(float _deltaTime)
{
	currentMoney += _deltaTime * earnMoneySpeed;

	if (currentMoney > maxMoney)
		currentMoney = maxMoney;

	if (hud)
	{
		hud->UpdatePlayerMoney(currentMoney, maxMoney, workerCatUpgradeCost, allUnits);
	}
}

void ABC_Player::UpgradeWorkerCat()
{
	if (workerCatCurrentLevel >= workerCatMaxLevel || currentMoney < workerCatUpgradeCost)
	{
		// Le nombre maximal d'amélioration est atteint ou pas assez d'argent pour l'amélioration.
		return;
	}

	currentMoney -= workerCatUpgradeCost;

	//Upgrade les stats d'argents
	workerCatUpgradeCost += workerCatUpgradeCostToAdd;
	maxMoney += addMaxMoney;
	earnMoneySpeed += addEarnMoneySpeed;
	
	++workerCatCurrentLevel;

	onUpgradeComplete.Broadcast();

	if (hud)
	{
		hud->UpdateWorkerCatPrice(workerCatCurrentLevel, workerCatMaxLevel, workerCatUpgradeCost);
	}
}

void ABC_Player::SpawnCat(int _catIndex)
{
	if (!unitBase)return;

	// Check si l'index est valide.
	const int _count = allUnits.Num();
	if (_catIndex < 0 || _catIndex >= _count)return;

	// Check si l'unité est valide.
	const TSubclassOf<ABC_Unit>& _unitToSpawn = allUnits[_catIndex];
	ABC_Unit* _unit = _unitToSpawn.GetDefaultObject();
	
	if (!_unit)return;

	// Check si le joueur a assez d'argent.
	const int _unitPrice = _unit->GetUnitPrice();
	if (currentMoney < _unitPrice)return;
	currentMoney -= _unitPrice;
	
	if (hud)
		hud->UpdateWorkerCatPrice(workerCatCurrentLevel, workerCatMaxLevel, workerCatUpgradeCost);


	// Spawn l'Unit au niveau de la base avec un offset de profondeur.
	const FVector& _spawnDeepOffset = FVector(FMath::RandRange(-spawnDeepOffset, -0.01f), 0.0f, 0.0f);

	ABC_Unit* _spawnedUnit = GetWorld()->SpawnActor<ABC_Unit>(_unitToSpawn,
															  unitBase->GetActorLocation() + _spawnDeepOffset,
															  FRotator::ZeroRotator);

	if (!_spawnedUnit)return;
	onUnitSpawned.Broadcast(_spawnedUnit);
}