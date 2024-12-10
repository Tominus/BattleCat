#include "BC_FightWidget.h"
#include "BC_CatSpawnerWidget.h"
#include "BC_FightHUD.h"
#include "BC_Unit.h"
#include "PaperSprite.h"
#include "BC_Macro.h"

#define MAX_CAT_SPAWN_PER_ROW 5
#define MAX_CAT 10


void UBC_FightWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitButtonsEvents();
	InitCatSpawners();
}

void UBC_FightWidget::InitButtonsEvents()
{
	playerController = GetWorld()->GetFirstPlayerController();
	if (!playerController)return;
	hud = playerController->GetHUD<ABC_FightHUD>();
	if (!hud)return;
	
	upgradeButton->OnClicked.AddUniqueDynamic(hud, &ABC_FightHUD::UpgradeButtonPressed);
	canonButton->OnClicked.AddUniqueDynamic(hud, &ABC_FightHUD::CanonButtonPressed);
}

void UBC_FightWidget::InitCatSpawners()
{
	if (!catSpawnerWidgetToSpawn.Get())
	{
		LOG_ERROR("UBC_FightWidget::InitCatSpawners-> Incomplete TSubclass");
		return;
	}

	if (!playerController || !hud)return;

	// Fait spawn 5 boutons et leur donne l'hud pour appelé les event + leur index de spawn.
	// (pour envoyer au player qu'elle Unit il faut spawn)
	for (int i = 0; i < MAX_CAT_SPAWN_PER_ROW; ++i)
	{
		UBC_CatSpawnerWidget* _catSpawnerWidget = CreateWidget<UBC_CatSpawnerWidget>(playerController, catSpawnerWidgetToSpawn);
		if (!_catSpawnerWidget)return;
		
		_catSpawnerWidget->SetupCatSpawner(hud, i);
		_catSpawnerWidget->AddToViewport();
		
		catSpawnersTopHorizontalBox->AddChild(_catSpawnerWidget);
		allCatSpawners.Add(_catSpawnerWidget);
	}

	// Refait spawn 5 boutons pour la secondes barre horizontale. (Avec un index + 5 (ceux qui ont déjà été spawn))
	for (int i = 0; i < MAX_CAT_SPAWN_PER_ROW; ++i)
	{
		UBC_CatSpawnerWidget* _catSpawnerWidget = CreateWidget<UBC_CatSpawnerWidget>(playerController, catSpawnerWidgetToSpawn);
		if (!_catSpawnerWidget)return;
		
		_catSpawnerWidget->SetupCatSpawner(hud, i + MAX_CAT_SPAWN_PER_ROW);
		_catSpawnerWidget->AddToViewport();
		
		catSpawnersBotHorizontalBox->AddChild(_catSpawnerWidget);
		allCatSpawners.Add(_catSpawnerWidget);
	}
}

void UBC_FightWidget::InitSpawnCatButtons(const TArray<TSubclassOf<ABC_Unit>>& _allUnits)
{
	int _count = _allUnits.Num();
	if (_count > MAX_CAT)
	{
		LOG_ERROR("UBC_FightWidget::InitSpawnCatButtons-> Too much cat to spawn");
		return;
	}

	// Pour toutes les Units que je lui envoie, je récupère leur Texture2D et l'envoie dans les boutons.
	for (int i = 0; i < _count; ++i)
	{
		ABC_Unit* _unitBP = Cast<ABC_Unit>(_allUnits[i].GetDefaultObject());
		if (!_unitBP)return;
		UTexture2D* _logoTexture = _unitBP->GetLogoTexture();
		RETURN_LOG(!_logoTexture, "UBC_FightWidget::InitSpawnCatButtons -> Missing Logo Texture");
		allCatSpawners[i]->AddCatSpawnerData(_logoTexture, _unitBP->GetUnitPrice());
	}

	// Tous les boutons qui n'ont pas d'Unit associer sont alors désactivé.
	const int _allCatSpawnersCount = allCatSpawners.Num();
	for (int i = _count; i < _allCatSpawnersCount; ++i)
	{
		allCatSpawners[i]->RemoveCatSpawnerData();
	}
}

void UBC_FightWidget::UpdateMoney(float _currentMoney, float _maxMoney, int _workerCatUpgradeMoney, const TArray<TSubclassOf<ABC_Unit>>& _allUnits)
{
	int _currentMoneyConverted = static_cast<int>(_currentMoney);
	int _maxMoneyConverted = static_cast<int>(_maxMoney);

	// Update l'affichage du text
	const FString& _moneyText = FString::FromInt(_currentMoneyConverted) + "/" + FString::FromInt(_maxMoneyConverted) + "$";
	
	moneyText->SetText(FText::FromString(_moneyText));

	
	// Update l'affichage du WorkerCat
	if (_currentMoney < _workerCatUpgradeMoney)
	{
		workerCatImage->SetBrushFromTexture(workerCatOffTexture);
	}
	else
	{
		workerCatImage->SetBrushFromTexture(workerCatOnTexture);
	}

	const int _count = _allUnits.Num();
	if (_count > allCatSpawners.Num())return;
	
	// Update l'affichage des boutons de spawn d'Units.
	for (int i = 0; i < _count; ++i)
	{
		ABC_Unit* _unitBP = Cast<ABC_Unit>(_allUnits[i].GetDefaultObject());
		if (!_unitBP)return;
		
		allCatSpawners[i]->SetCatSpawnerVisibility(_unitBP->GetUnitPrice() > _currentMoneyConverted);
	}
}

void UBC_FightWidget::UpdateWorkerCatPrice(int _workerCatLevel, int _workerCatMaxLevel, int _workerCatPrice)
{
	if (_workerCatLevel >= _workerCatMaxLevel)
	{
		workerCatLevelText->SetText(FText::FromString("Level: MAX"));
		workerCatPriceText->SetText(FText::FromString(""));
	}
	else
	{
		const FString& _levelText = "Level: " + FString::FromInt(_workerCatLevel);
		const FString& _priceText = FString::FromInt(_workerCatPrice) + "$";
		
		workerCatLevelText->SetText(FText::FromString(_levelText));
		workerCatPriceText->SetText(FText::FromString(_priceText));
	}
}