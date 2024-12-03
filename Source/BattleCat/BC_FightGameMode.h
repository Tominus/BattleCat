#pragma once

#include "CoreMinimal.h"
#include "BC_Player.h"
#include "BC_AI_Enemy.h"
#include "GameFramework/GameModeBase.h"
#include "BC_FightGameMode.generated.h"


/* Le FightGameMode stock le player et l'AI qui fait apparaitre les chats.
 * Il permet de faire une pont entre ces variables et les ceux qui en ont besoin. */

UCLASS()
class BATTLECAT_API ABC_FightGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY()
	ABC_Player* player = nullptr;
	
	UPROPERTY()
	ABC_AI_Enemy* enemyAI = nullptr;
	

public:
	FORCEINLINE void SetPlayer(ABC_Player* _player) { player = _player; }
	FORCEINLINE void SetAIEnemy(ABC_AI_Enemy* _enemyAI) { enemyAI = _enemyAI; }
	FORCEINLINE ABC_Player* GetPlayer() const { return player; }
	FORCEINLINE ABC_AI_Enemy* GetAIEnemy() const { return enemyAI; }
	
};