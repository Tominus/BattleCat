#include "BC_Enemy.h"

#include "BC_FightGameMode.h"
#include "BC_Unit.h"


void ABC_Enemy::BeginPlay()
{
	Super::BeginPlay();
	InitEnemy();
}

void ABC_Enemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DetectEnemy();
	Move(DeltaSeconds);
	Attacking(DeltaSeconds);
}

void ABC_Enemy::Attack_Single()
{
	ABC_Entity* _entity = Cast<ABC_Entity>(hitSingleResult.GetActor());
	if (!_entity)return;
	_entity->LoseHealth(attackDamage, enemyTraitTypes);
}

void ABC_Enemy::Attack_Area()
{
	const size_t& _count = hitMultiResult.Num();
	for (size_t i = 0; i < _count; ++i)
	{
		ABC_Entity* _entity = Cast<ABC_Entity>(hitMultiResult[i].GetActor());
		if (!_entity)return;
		_entity->LoseHealth(attackDamage, enemyTraitTypes);
	}
}

void ABC_Enemy::InitEnemy()
{
	RETURN_LOG(enemyTraitTypes.Num() < 1, "ABC_Enemy::InitEnemy -> Enemy with no Traits detected !");
	
	onEntityDie.AddUniqueDynamic(this, &ABC_Enemy::DropMoney);
}

void ABC_Enemy::DropMoney()
{
	ABC_FightGameMode* _gameMode = GetWorld()->GetAuthGameMode<ABC_FightGameMode>();
	if (!_gameMode)return;
	ABC_Player* _player = _gameMode->GetPlayer();
	if (!_player)return;
	_player->AddPlayerMoney(moneyDrop);
}