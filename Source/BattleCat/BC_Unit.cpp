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

void ABC_Unit::LoseHealth(int _damageAmount, const TArray<ETraitType>& _attackerTraitsTypes)
{
	// Calculate
	float _damageMultiplier = 1.0f;

	CalculateDamageTakenByTraits(_attackerTraitsTypes, _damageMultiplier);

	_damageAmount *= _damageMultiplier;
	
	Super::LoseHealth(_damageAmount, _attackerTraitsTypes);
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
		_enemy->LoseHealth(attackDamage * _damageMultiplier, attackTraitsType);
	}
	else if (ABC_EnemyBase* _enemyBase = Cast<ABC_EnemyBase>(_enemyActor))
	{
		_enemyBase->LoseHealth(attackDamage, attackTraitsType);
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
	for (size_t i = 0; i < _attackEffectsCount; ++i)
	{
		const EEffectType _currentAttackEffectType = attackEffectsType[i];
		
		if (_currentAttackEffectType == EEffectType::STRONG_AGAINST)
		{
			_damageMultiplier = 1.5f;
		}
		else if (_currentAttackEffectType == EEffectType::MASSIVE_DAMAGE)
		{
			_damageMultiplier = 3.0f;
		}
		else if (_currentAttackEffectType == EEffectType::INSANE_DAMAGE)
		{
			_damageMultiplier = 5.0f;
		}
	}
}

void ABC_Unit::CalculateEffectApplyed(TArray<EEffectType>& _effectsToApply)
{
	// TODO
}

void ABC_Unit::CalculateDamageTakenByTraits(const TArray<ETraitType>& _attackerTraitsType, float& _damageMultiplier)
{
	const size_t _attackerTraitsCount = _attackerTraitsType.Num();
	for (size_t i = 0; i < _attackerTraitsCount; ++i)
	{
		const ETraitType _enemyTrait = _attackerTraitsType[i];
			
		const size_t _attackTraitsCount = attackTraitsType.Num();
		for (size_t j = 0; j < _attackTraitsCount; ++j)
		{
			if (_enemyTrait == attackTraitsType[j])
			{
				// Unit being attacked by the right attacker type

				CalculateDamageTaken(_damageMultiplier);
			}
		}
	}
}

void ABC_Unit::CalculateDamageTaken(float& _damageMultiplier)
{
	const size_t _attackEffectsCount = attackEffectsType.Num();
	for (size_t i = 0; i < _attackEffectsCount; ++i)
	{
		const EEffectType _currentAttackEffectType = attackEffectsType[i];
		
		if (_currentAttackEffectType == EEffectType::STRONG_AGAINST)
		{
			_damageMultiplier = 0.5f;
		}
		else if (_currentAttackEffectType == EEffectType::RESISTANT)
		{
			_damageMultiplier = 0.25f;
		}
		else if (_currentAttackEffectType == EEffectType::INSANELY_TOUGH)
		{
			_damageMultiplier = 0.167f;
		}
	}
}