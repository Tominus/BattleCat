#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "BC_CatSpawnerWidget.generated.h"

class ABC_FightHUD;

/* CatSpawnerWidget permet de faire la liaison entre le rendu de l'UI et l'unité spawné. */

UCLASS()
class BATTLECAT_API UBC_CatSpawnerWidget : public UUserWidget
{
	GENERATED_BODY()

	// Image de contour d'arrière plan.
	UPROPERTY(meta = (BindWidget)) UImage* catSpawnerImage = nullptr;

	// Image de l'unité.
	UPROPERTY(meta = (BindWidget)) UImage* catImage = nullptr;

	// Image permettant de grisé le bouton (le rendant inclickable au passage)
	UPROPERTY(meta = (BindWidget)) UImage* greyImage = nullptr;
	
	UPROPERTY(meta = (BindWidget)) UTextBlock* catPriceText = nullptr;

	UPROPERTY(meta = (BindWidget)) UButton* catSpawnerButton = nullptr;

	UPROPERTY()
	int catSpawnerIndex = -1;

	UPROPERTY()
	ABC_FightHUD* hud = nullptr;

private:
	virtual void NativeConstruct() override;

	// Fonction appelée quand on click sur le bouton pour faire une Unité. Appelle un event de l'HUD.
	UFUNCTION() void OnCatSpawnerButtonClicked();

public:
	void SetupCatSpawner(ABC_FightHUD* _hud, int _catSpawnerIndex);

	// Update la texture de l'image avec celle de l'unité et son prix.
	void AddCatSpawnerData(UTexture2D* _texture, int _unitPrice);

	// Désactive le rendu de l'image et du text quand aucune unité n'est liée.
	void RemoveCatSpawnerData();

	// Grise ou non le rendu du bouton. (Le rendant inclickable)
	void SetCatSpawnerVisibility(bool _isGrey);
};