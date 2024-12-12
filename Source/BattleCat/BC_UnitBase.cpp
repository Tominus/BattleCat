#include "BC_UnitBase.h"


void ABC_UnitBase::LoseHealth(int _damageAmount, const TArray<ETraitType>& _attackerTraitsType)
{
	if (isDead)return;
	
	healthPoint -= _damageAmount;

	onLoseHealth.Broadcast();

	if (healthPoint <= 0)
	{
		healthPoint = 0;
		isDead = true;
		onUnitBaseDestroyed.Broadcast();
	}
}