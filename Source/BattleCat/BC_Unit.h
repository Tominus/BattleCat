#pragma once

#include "CoreMinimal.h"
#include "BC_Entity.h"
#include "BC_Unit.generated.h"


/* L'Unit est une class fille d'Entity, elle définit ses actions en Tick, ainsi qu'une Attack propre à elle même.
 * Les Units sont les class spawnées par le player et détiennent un prix de spawn. */


UCLASS()
class BATTLECAT_API ABC_Unit : public ABC_Entity
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Entity|Unit|Fonctionnality") UTexture2D* logoTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Entity|Unit|Stats") int unitPrice = 0;
	UPROPERTY(EditAnywhere, Category = "Entity|Unit|Stats") TArray<ETraitType> attackTraitsType;
	UPROPERTY(EditAnywhere, Category = "Entity|Unit|Stats") TArray<EEffectType> attackEffectsType;

public:
	FORCEINLINE UTexture2D* GetLogoTexture() const { return logoTexture; } 
	FORCEINLINE int GetUnitPrice() const { return unitPrice; }

protected:
	// Définit ce que l'unité fait. (Detection, Movement et phase d'Attack)
	virtual void Tick(float DeltaSeconds) override;
	
	// Redéfinit la fonction Attack de la class mère (ABC_Entity) pour ajouter les fonctionnalitées voulues.
	virtual void Attack_Single() override;
	virtual void Attack_Area() override;
	
	virtual void LoseHealth(int _damageAmount, const TArray<ETraitType>& _attackerTraitsTypes) override;

	void AttackEnemy(AActor* _enemyActor);

	void CalculateDamageAndEffectsApplyed(const TArray<ETraitType>& _enemyTraitsType, float& _damageMultiplier,
										 TArray<EEffectType>& _effectsToApply);

	void CalculateDamageApplyed(float& _damageMultiplier);
	void CalculateEffectApplyed(TArray<EEffectType>& _effectsToApply);

	void CalculateDamageTakenByTraits(const TArray<ETraitType>& _attackerTraitsType, float& _damageMultiplier);
	void CalculateDamageTaken(float& _damageMultiplier);
};