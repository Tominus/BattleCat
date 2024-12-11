#include "BC_Unit.h"

#include "BC_Enemy.h"
#include "BC_EnemyBase.h"


void ABC_Unit::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DetectEnemy();
	Move(DeltaSeconds);
	Attacking(DeltaSeconds);
}

void ABC_Unit::Attack_Single()
{
	AttackEnemy(hitSingleResult.GetActor());
}

void ABC_Unit::Attack_Area()
{
	const size_t& _count = hitMultiResult.Num();
	for (size_t i = 0; i < _count; ++i)
	{
		AttackEnemy(hitMultiResult[i].GetActor());
	}
}

void ABC_Unit::AttackEnemy(AActor* _enemyActor)
{
	if (!_enemyActor)return;
	
	if (ABC_Enemy* _enemy = Cast<ABC_Enemy>(_enemyActor))
	{
		float _damageMultiplier = 1.0f;
		TArray<EEffectType> _effectsToApply;

		CalculateDamageAndEffectsApplyed(_enemy->GetEnemyTraitTypes(), _damageMultiplier, _effectsToApply);
		
		// TODO apply effects to enemy
		_enemy->LoseHealth(attackDamage * _damageMultiplier);
	}
	else if (ABC_EnemyBase* _enemyBase = Cast<ABC_EnemyBase>(_enemyActor))
	{
		_enemyBase->LoseHealth(attackDamage);
	}
}

void ABC_Unit::CalculateDamageAndEffectsApplyed(const TArray<ETraitType>& _enemyTraitsType, float& _damageMultiplier,
                                               TArray<EEffectType>& _effectsToApply)
{
	const size_t _enemyTraitsCount = _enemyTraitsType.Num();
	for (size_t i = 0; i < _enemyTraitsCount; ++i)
	{
		const ETraitType _enemyTrait = _enemyTraitsType[i];
			
		const size_t _attackTraitsCount = attackTraitsType.Num();
		for (size_t j = 0; j < _attackTraitsCount; ++j)
		{
			if (_enemyTrait == attackTraitsType[j])
			{
				// Unit is attacking an enemy using Effects
				CalculateDamageApplyed(_damageMultiplier);
				CalculateEffectApplyed(_effectsToApply);
			}
		}
	}
}

void ABC_Unit::CalculateDamageApplyed(float& _damageMultiplier)
{
	const size_t _attackEffectsCount = attackEffectsType.Num();
	for (size_t k = 0; k < _attackEffectsCount; ++k)
	{
		if (attackEffectsType[k] == EEffectType::STRONG_AGAINST)
		{
			_damageMultiplier = 1.5f;
		}
	}
}

void ABC_Unit::CalculateEffectApplyed(TArray<EEffectType>& _effectsToApply)
{
	// TODO
}