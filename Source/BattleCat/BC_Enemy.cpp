#include "BC_Enemy.h"

#include "BC_Unit.h"


void ABC_Enemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DetectEnemy();
	Move(DeltaSeconds);
	Attacking(DeltaSeconds);
}

void ABC_Enemy::Attack()
{
	ABC_Entity* _unit = Cast<ABC_Entity>(hitResult.GetActor());
	if (!_unit)return;
	_unit->LoseHealth(attackDamage);
}