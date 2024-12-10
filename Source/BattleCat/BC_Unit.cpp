#include "BC_Unit.h"

#include "BC_Enemy.h"


void ABC_Unit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DetectEnemy();
	Move(DeltaSeconds);
	Attacking(DeltaSeconds);
}

void ABC_Unit::Attack_Single()
{
	ABC_Entity* _entity = Cast<ABC_Entity>(hitSingleResult.GetActor());
	if (!_entity)return;
	_entity->LoseHealth(attackDamage);
}

void ABC_Unit::Attack_Area()
{
	const size_t& _count = hitMultiResult.Num();
	for (size_t i = 0; i < _count; ++i)
	{
		ABC_Entity* _entity = Cast<ABC_Entity>(hitMultiResult[i].GetActor());
		if (!_entity)return;
		_entity->LoseHealth(attackDamage);
	}
}