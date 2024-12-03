#include "BC_EnemyBase.h"


void ABC_EnemyBase::LoseHealth(int _damageAmount)
{
	if (isDead)return;
	
	healthPoint -= _damageAmount;

	onLoseHealth.Broadcast();

	if (healthPoint <= 0)
	{
		healthPoint = 0;
		isDead = true;
		onEnemyBaseDestroyed.Broadcast();
	}
}