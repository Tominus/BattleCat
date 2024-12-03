#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BC_AI_Enemy.generated.h"

class ABC_EnemyBase;
class ABC_Enemy;


/* Structure de données permettant de stocker quelle unité l'AI enemy va faire spawn,
 * ainsi que leurs delay. */

USTRUCT()
struct FAI_SpawnData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float firstSpawnTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	float spawnTime = 0.0f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABC_Enemy> enemyToSpawn;

	UPROPERTY() bool hasFirstSpawn = false;
	UPROPERTY() float currentTime = 0.0f;
};


/* AI_Enemy est "l'AI" du jeux permettant de faire apparettre des Enemys suivant des conditions.
 * Elle stock un tableau de données de spawn d'Enemy et un offset pour la profondeur de spawn. */

UCLASS()
class BATTLECAT_API ABC_AI_Enemy : public AActor
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySpawned, ABC_Enemy*, _enemy);

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnEnemySpawned onEnemySpawned;

private:
	// Array de donnée de spawn. Permet en BP de pouvoir ajouter autant de données de spawn que nécessaire.
	UPROPERTY(EditAnywhere, Category = "AIEnemy")
	TArray<FAI_SpawnData> spawnData;

	// Permet le décallage lors du spawn des Enemy pour éviter la superposition de ceux-ci.
	UPROPERTY(EditAnywhere, Category = "AIEnemy", meta = (ClampMin = "1.0", ClampMax = "30.0"))
	float spawnDeepOffset = 30.0f;
	
	UPROPERTY()
	ABC_EnemyBase* enemyBase = nullptr;

public:
	FORCEINLINE void SetEnemyBase(ABC_EnemyBase* _enemyBase) { enemyBase = _enemyBase; }
	
public:
	ABC_AI_Enemy();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Envoie cette AI dans le gamemode pour que le FightStarter puisse lui donner l'EnemyBase.
	void InitAI();

	// Calcul en Tick chaque élément de SpawnData présent dans l'Array.
	void UpdateAI(float _deltaTime);

	// Fonction faisant spawn l'enemy donné en paramètre avec un offset de profondeur.
	void SpawnEnemy(const TSubclassOf<ABC_Enemy>& _enemyToSpawn);
};