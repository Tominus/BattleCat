#include "BC_AI_Enemy.h"

#include "BC_Enemy.h"
#include "BC_EnemyBase.h"
#include "BC_FightGameMode.h"


ABC_AI_Enemy::ABC_AI_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABC_AI_Enemy::BeginPlay()
{
	Super::BeginPlay();
	InitAI();
}

void ABC_AI_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAI(DeltaTime);
}

void ABC_AI_Enemy::InitAI()
{
	ABC_FightGameMode* _gm = GetWorld()->GetAuthGameMode<ABC_FightGameMode>();
	if (!_gm)return;
	_gm->SetAIEnemy(this);
}

void ABC_AI_Enemy::UpdateAI(float _deltaTime)
{
	const int _count = spawnData.Num();
	for (int i = 0; i < _count; ++i)
	{
		// Parcours chaque SpawnData présent dans l'Array
		FAI_SpawnData& _spawnData = spawnData[i];

		// Pour chacun d'eux, le temps s'écoule normalement
		_spawnData.currentTime += _deltaTime;

		// On regarde si une unité a déjà été spawné.
		if (!_spawnData.hasFirstSpawn)
		{
			// Si aucune unité spawné alors notre MaxDelay est le .firstSpawnTime
			if (_spawnData.currentTime > _spawnData.firstSpawnTime)
			{
				_spawnData.hasFirstSpawn = true;
				_spawnData.currentTime = 0.0f;
				SpawnEnemy(_spawnData.enemyToSpawn);
			}
		}
		else
		{
			// Si une unité a déjà été spawné alors notre MaxDelay est le .spawnTime
			if (_spawnData.currentTime > _spawnData.spawnTime)
			{
				_spawnData.currentTime = 0.0f;
				SpawnEnemy(_spawnData.enemyToSpawn);
			}
		}
	}
}

void ABC_AI_Enemy::SpawnEnemy(const TSubclassOf<ABC_Enemy>& _enemyToSpawn)
{
	if (!enemyBase)return;

	// Spawn l'enemy avec un décallage de profondeur au niveau de la base.
	const FVector& _spawnDeepOffset = FVector(FMath::RandRange(-spawnDeepOffset, -0.01f), 0.0f, 0.0f);

	ABC_Enemy* _enemy = GetWorld()->SpawnActor<ABC_Enemy>(_enemyToSpawn,
													      enemyBase->GetActorLocation() + _spawnDeepOffset,
													      FRotator::ZeroRotator);

	if (!_enemy)return;
	
	onEnemySpawned.Broadcast(_enemy);
}