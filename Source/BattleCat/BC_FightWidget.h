#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "BC_FightWidget.generated.h"


/* Le FightWidget stock toutes les données visuelles de l'interface ainsi que leurs Update. */

UCLASS()
class BATTLECAT_API UBC_FightWidget : public UUserWidget
{
	GENERATED_BODY()

	// Détient le BP de Widget à faire spawn pour chaqu'un des boutons à spawn d'Units.
	UPROPERTY(EditAnywhere, Category = "FightWidget")
	TSubclassOf<class UBC_CatSpawnerWidget> catSpawnerWidgetToSpawn;

	// Textures utilisées pour griser ou non le WorkerCat suivant l'argent du joueur.
	UPROPERTY(EditAnywhere, Category = "FightWidget")
	UTexture2D* workerCatOffTexture = nullptr;
	UPROPERTY(EditAnywhere, Category = "FightWidget")
	UTexture2D* workerCatOnTexture = nullptr;
	

	// NOT USED
	UPROPERTY(meta = (BindWidget)) UButton* upgradeButton = nullptr;
	UPROPERTY(meta = (BindWidget)) UButton* canonButton = nullptr;
	
	UPROPERTY(meta = (BindWidget)) UTextBlock* moneyText = nullptr;

	// Les deux barres horizontales où on fait spawn nos boutons pour le spawn d'Units.
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* catSpawnersTopHorizontalBox = nullptr;
	UPROPERTY(meta = (BindWidget)) UHorizontalBox* catSpawnersBotHorizontalBox = nullptr;

	// L'image où on change l'affichage grâce aux deux Textures2D On et Off.
	UPROPERTY(meta = (BindWidget)) UImage* workerCatImage = nullptr;
	
	UPROPERTY(meta = (BindWidget)) UTextBlock* workerCatLevelText = nullptr;
	UPROPERTY(meta = (BindWidget)) UTextBlock* workerCatPriceText = nullptr;

	UPROPERTY()	class ABC_FightHUD* hud = nullptr;
	UPROPERTY()	APlayerController* playerController = nullptr;

	// Stock tous les boutons de spawn d'Units créé pour changer leur affichage.
	UPROPERTY() TArray<UBC_CatSpawnerWidget*> allCatSpawners;
	
private:
	virtual void NativeConstruct() override;

	void InitButtonsEvents();

	// Init les boutons permettant de spawn des Units. 
	void InitCatSpawners();


public:
	// Rajoute sur les boutons de spawn d'Unit l'affichage des unitées et leur coût.
	void InitSpawnCatButtons(const TArray<TSubclassOf<class ABC_Unit>>& _allUnits);

	// Update le rendu de l'argent, de quelle Unit je peux acheter et du WorkerCat.
	void UpdateMoney(float _currentMoney, float _maxMoney, int _workerCatUpgradeMoney, const TArray<TSubclassOf<ABC_Unit>>& _allUnits);

	// Update le rendu à chaque amélioration du WorkerCat (prix et level)
	void UpdateWorkerCatPrice(int _workerCatLevel, int _workerCatMaxLevel, int _workerCatPrice);
};