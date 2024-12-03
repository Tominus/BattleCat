#include "BC_FightStarter.h"

#include "BC_UnitBase.h"
#include "BC_EnemyBase.h"
#include "BC_FightGameMode.h"


ABC_FightStarter::ABC_FightStarter()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABC_FightStarter::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle _timerHandle;
	FTimerManager& _timerManager = GetWorld()->GetTimerManager();
	_timerManager.SetTimer(_timerHandle, this, &ABC_FightStarter::InitGame, 0.01f);
}

void ABC_FightStarter::InitGame()
{
	if (!enemyBaseToSpawn.Get() || !unitBaseToSpawn.Get())
	{
		UE_LOG(LogTemp, Error, TEXT("ABC_FightStarter::InitGame -> Invalid TSubclassOf"));
		return;
	}

	enemyBase = GetWorld()->SpawnActor<ABC_EnemyBase>(enemyBaseToSpawn, FVector::LeftVector * basesSpacing * 0.5, FRotator::ZeroRotator);
	unitBase = GetWorld()->SpawnActor<ABC_UnitBase>(unitBaseToSpawn, FVector::RightVector * basesSpacing * 0.5, FRotator::ZeroRotator);

	ABC_FightGameMode* _gm = GetWorld()->GetAuthGameMode<ABC_FightGameMode>();
	if (!_gm)return;
	ABC_Player* _player = _gm->GetPlayer();
	if (!_player)return;
	_player->SetUnitBase(unitBase);

	ABC_AI_Enemy* _enemyAI = _gm->GetAIEnemy();
	if (!_enemyAI)return;
	_enemyAI->SetEnemyBase(enemyBase);
}