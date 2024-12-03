#include "BC_FightHUD.h"

#include "Blueprint/UserWidget.h"
#include "BC_FightWidget.h"


void ABC_FightHUD::BeginPlay()
{
	Super::BeginPlay();
	InitHUDWidgets();
}

void ABC_FightHUD::InitHUDWidgets()
{
	playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController)return;

	fightWidget = CreateWidget<UBC_FightWidget>(playerController, fightWidgetToSpawn);
	if (!fightWidget)return;
	fightWidget->AddToViewport();
}

void ABC_FightHUD::InitSpawnCatButtons(const TArray<TSubclassOf<ABC_Unit>>& _allUnits)
{
	if (!fightWidget)return;
	fightWidget->InitSpawnCatButtons(_allUnits);
}

void ABC_FightHUD::UpdatePlayerMoney(float _currentMoney, float _maxMoney, int _workerCatUpgradeMoney, const TArray<TSubclassOf<ABC_Unit>>& _allUnits)
{
	if (!fightWidget)return;
	fightWidget->UpdateMoney(_currentMoney, _maxMoney, _workerCatUpgradeMoney, _allUnits);
}

void ABC_FightHUD::UpdateWorkerCatPrice(int _workerCatLevel, int _workerCatMaxLevel, int _workerCatPrice)
{
	if (!fightWidget)return;
	fightWidget->UpdateWorkerCatPrice(_workerCatLevel, _workerCatMaxLevel, _workerCatPrice);
}

void ABC_FightHUD::UpgradeButtonPressed()
{
	onUpgradeButtonPressed.Broadcast();
}

void ABC_FightHUD::CanonButtonPressed()
{
	onCanonButtonPressed.Broadcast();
}

void ABC_FightHUD::SpawnCatButtonPressed(int _buttonIndex)
{
	onSpawnCatButtonPressed.Broadcast(_buttonIndex);
}