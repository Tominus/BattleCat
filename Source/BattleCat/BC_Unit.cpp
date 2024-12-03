#include "BC_Unit.h"

#include "BC_Enemy.h"


void ABC_Unit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DetectEnemy();
	Move(DeltaSeconds);
	Attacking(DeltaSeconds);
}

void ABC_Unit::Attack()
{
	ABC_Entity* _enemy = Cast<ABC_Entity>(hitResult.GetActor());
	if (!_enemy)return;
	_enemy->LoseHealth(attackDamage);
}