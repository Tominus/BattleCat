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

	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int unitPrice = 0;

public:
	FORCEINLINE int GetUnitPrice() const { return unitPrice; }

protected:
	// Définit ce que l'unité fait. (Detection, Movement et phase d'Attack)
	virtual void Tick(float DeltaSeconds) override;
	
	// Redéfinit la fonction Attack de la class mère (ABC_Entity) pour ajouter les fonctionnalitées voulues.
	virtual void Attack() override;
};