#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BC_FightStarter.generated.h"

class ABC_UnitBase;
class ABC_EnemyBase;

/* Le FightStarter fait apparettre les bases des deux camps opposées.
 * Une fois les deux bases spawnées, il les donne au player et à l'AI
 * pour qu'ils puissent faire apparettre leurs unitées à la position des bases. */

UCLASS()
class BATTLECAT_API ABC_FightStarter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "FightStarter")
	TSubclassOf<ABC_UnitBase> unitBaseToSpawn;
	
	UPROPERTY(EditAnywhere, Category = "FightStarter")
	TSubclassOf<ABC_EnemyBase> enemyBaseToSpawn;
	
	UPROPERTY(EditAnywhere, Category = "FightStarter")
	float basesSpacing = 100.0f;

	UPROPERTY()	ABC_EnemyBase* enemyBase = nullptr;
	UPROPERTY()	ABC_UnitBase* unitBase = nullptr;
	
public:
	ABC_FightStarter();

protected:
	virtual void BeginPlay() override;

private:
	/* Spawn des bases + link au player et l'AI.
	 * Fonction appelée avec delay pour que le GameMode est le temps d'être setup. */
	UFUNCTION() void InitGame();
};