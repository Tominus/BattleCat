#pragma once

#include "CoreMinimal.h"
#include "BC_Entity.h"
#include "BC_EnemyBase.generated.h"


/* Class de la Base de l'Enemy.
 * Utilisé comme point de spawn et comme unité à détruire pour gagner la partie.
 * Un event est présent pour dire que la base est détruite pour ensuite faire gagner le joueur. */

UCLASS()
class BATTLECAT_API ABC_EnemyBase : public ABC_Entity
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyBaseDestroyed);

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnEnemyBaseDestroyed onEnemyBaseDestroyed;

public:
	FORCEINLINE FOnEnemyBaseDestroyed& OnEnemyBaseDestroyed() { return onEnemyBaseDestroyed; }

protected:
	// Redéfinition dont la façon la base prend des dégâts pour éviter le Destroy() et faire un Event à la place.
	virtual void LoseHealth(int _damageAmount) override;
	
};