#include "BC_CatSpawnerWidget.h"

#include "BC_FightHUD.h"


void UBC_CatSpawnerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	catSpawnerButton->OnClicked.AddUniqueDynamic(this, &UBC_CatSpawnerWidget::OnCatSpawnerButtonClicked);
}

void UBC_CatSpawnerWidget::OnCatSpawnerButtonClicked()
{
	if (!hud)return;

	hud->SpawnCatButtonPressed(catSpawnerIndex);
}

void UBC_CatSpawnerWidget::SetupCatSpawner(ABC_FightHUD* _hud, int _catSpawnerIndex)
{
	hud = _hud;
	catSpawnerIndex = _catSpawnerIndex;
}

void UBC_CatSpawnerWidget::AddCatSpawnerData(UTexture2D* _texture, int _unitPrice)
{
	FString _text = FString::FromInt(_unitPrice) + "$";
	catPriceText->SetText(FText::FromString(_text));

	// Update l'image de l'unité avec la nouvelle texture. (et change sa taille)
	catImage->SetBrushFromTexture(_texture, true);
}

void UBC_CatSpawnerWidget::RemoveCatSpawnerData()
{
	catPriceText->SetText(FText::FromString(""));
	catImage->SetVisibility(ESlateVisibility::Hidden);
}

void UBC_CatSpawnerWidget::SetCatSpawnerVisibility(bool _isGrey)
{
	greyImage->SetVisibility(_isGrey ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
