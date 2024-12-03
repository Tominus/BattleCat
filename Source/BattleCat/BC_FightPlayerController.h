#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BC_FightPlayerController.generated.h"


/* Ce FightPlayerController sert uniquement à activer la visibilitée de la souris du player en BeginPlay() */

UCLASS()
class BATTLECAT_API ABC_FightPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	virtual void BeginPlay() override;

	/* Active la visibilité du curseur. */
	void ShowMouseCursor();
};