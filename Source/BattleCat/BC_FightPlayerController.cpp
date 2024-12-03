#include "BC_FightPlayerController.h"

void ABC_FightPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowMouseCursor();
}

void ABC_FightPlayerController::ShowMouseCursor()
{
	SetShowMouseCursor(true);
}