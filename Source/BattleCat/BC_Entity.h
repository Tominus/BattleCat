#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "BC_Entity.generated.h"

class ABC_Player;

/* Enum permettant de setup l'animation et le temps d'attente pour l'Attack. */

UENUM()
enum class EAttackState
{
	NONE,
	FORSWING,
	BACKSWING,
};

/* Entity est une class contenant beaucoup d'information (Animation, Stats, ou fonctionnalitées).
 * Cette class a pour but que toutes ses class filles ont les mêmes Datas et Fonctions.
 * L'Entity ne fera aucune action d'elle même, seules ses class filles définissent ce qu'il se passe en Tick.*/

UCLASS()
class BATTLECAT_API ABC_Entity : public AActor
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttack);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoseHealth);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEntityDie);

protected:
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnAttack onAttack;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnLoseHealth onLoseHealth;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite)
	FOnEntityDie onEntityDie;

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollision = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* spriteComponent = nullptr;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Entity|Fonctionnality")	TArray<TEnumAsByte<EObjectTypeQuery>> attackLayer;
	UPROPERTY(EditAnywhere, Category = "Entity|Fonctionnality")	FVector forwardDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* walkSprite = nullptr;
	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* forswingSprite = nullptr;
	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* backswingSprite = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float moveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackForswingTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackBackswingTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int attackDamage = 0;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int attackRange = 0;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int healthPoint = 0;

	//TODO area damage, kb, money earn per kill (enemy), cooldown, %multiplier, win/death, (oui il reste beaucoup de trucs)

	UPROPERTY() bool isDead = false;
	UPROPERTY() bool isAttacking = false;
	UPROPERTY() bool hasFinishAttackAnimation = true;
	UPROPERTY() float currentAttackSpeedTimer = 0.0f;
	UPROPERTY() float currentAttackSwingTimer = 0.0f;
	UPROPERTY() EAttackState attackState = EAttackState::NONE;

	// Stock le résultat du LineTrace lors de la détection pour éviter dans refaire le même Tick.
	UPROPERTY() FHitResult hitResult;

public:
	FORCEINLINE UPaperSpriteComponent* GetSpriteComponent() const { return spriteComponent; }
	
public:
	ABC_Entity();

protected:
	virtual void BeginPlay() override;

	// Fonction virtual pour que les class filles puissent redéfinir son fonctionnement
	// Fait un LineTrace pour détecter si un enemy de cette entité est proche ou pas.
	virtual void DetectEnemy();

	// Fonction virtual pour que les class filles puissent redéfinir son fonctionnement
	// Déplace dans la direction voulue l'entité, uniquement lorsque l'entité n'attaque pas.
	virtual void Move(float _deltaTime);

	// Fonction virtual pour que les class filles puissent redéfinir son fonctionnement
	// Fonction permettant d'infliger des dégâts à l'enemy détecté. (Appelée automatiquement lors de la phase d'attack)
	virtual void Attack();

	// Fonctionnement de l'attack (Déroulement de l'animation, temps d'attente et appel de la fonction Attack)
	void Attacking(float _deltaTime);
	
public:

	// Fonction virtual pour que les class filles puissent redéfinir son fonctionnement
	// Permet de faire perdre des points de vie à l'unité et la détruire lors de la mort.
	virtual void LoseHealth(int _damageAmount);

private:
	// Permet de calculer la hauteur à laquelle le Sprite se positionne (Car toutes les entitées sont en (0, x, 0))
	void InitSpriteLocation();
};