#pragma once

#include "CoreMinimal.h"
#include "BC_Entity.h"
#include "BC_Enemy.generated.h"


/* L'Enemy est une class fille d'Entity, elle définit juste ses actions en Tick,
 * ainsi qu'une Attack propre à elle même.
 * Les Enemy sont les class spawnées par l'AI */

UCLASS()
class BATTLECAT_API ABC_Enemy : public ABC_Entity
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Entity|Enemy|Stats") int moneyDrop = 0;
	UPROPERTY(EditAnywhere, Category = "Entity|Enemy|Stats")
	TArray<ETraitType> enemyTraitTypes = TArray<ETraitType>
	({
		ETraitType::TRAITLESS
	});
	UPROPERTY(EditAnywhere, Category = "Entity|Enemy|Stats") TArray<EEffectType> enemyEffectType;
	

public:
	FORCEINLINE const TArray<ETraitType>& GetEnemyTraitTypes() const { return enemyTraitTypes; }
	
protected:
	virtual void BeginPlay() override;
	// Définit ce que l'enemy fait. (Detection, Movement et phase d'Attack)
	virtual void Tick(float DeltaSeconds) override;

	// Redéfinit la fonction Attack de la class mère (ABC_Entity) pour ajouter les fonctionnalitées voulues.
	virtual void Attack_Single() override;
	virtual void Attack_Area() override;

private:
	void InitEnemy();
	UFUNCTION() void DropMoney();
};