#pragma once

#include "CoreMinimal.h"
#include "BC_Entity.h"
#include "BC_UnitBase.generated.h"


/* Class de la Base du player.
 * Utilisé comme point de spawn et comme unité à détruire pour perdre la partie.
 * Un event est présent pour dire que la base est détruite pour ensuite faire perdre le joueur. */

UCLASS()
class BATTLECAT_API ABC_UnitBase : public ABC_Entity
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnitBaseDestroyed);

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnUnitBaseDestroyed onUnitBaseDestroyed;

public:
	FORCEINLINE FOnUnitBaseDestroyed& OnUnitBaseDestroyed() { return onUnitBaseDestroyed; }

protected:
	// Redéfinition dont la façon la base prend des dégâts pour éviter le Destroy() et faire un Event à la place.
	virtual void LoseHealth(int _damageAmount) override;
	
};