#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/Actor.h"
#include "BC_Macro.h"
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

UENUM()
enum class EAttackType
{
	SINGLE,
	AREA,
	//LONG_RANGE,
	//OMNI_STRIKE,
};

UENUM()
enum class ETraitType
{
	TRAITLESS,
	RED,
	FLOATING,
	BLACK,
	METAL,
	ANGEL,
	ALIEN,
	ZOMBIE,
	RELIC,
	AKU,
};

UENUM()
enum class EEffectType
{
	//WEAKEN,
	//FREEZE,
	//SLOW,
	//ATTACKS_ONLY,
	STRONG_AGAINST,
	//RESISTANT,
	//INSANELY_TOUGH,
	//MASSIVE_DAMAGE,
	//INSANE_DAMAGE,
	//KNOCKBACK,
	//WARP,
	//CURSE,
	//DODGE_ATTACK
	// TODO other effects types
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
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareAttack);

protected:
	// Event Used for BP song assignment and to use the right attack function.
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnAttack onAttack;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnLoseHealth onLoseHealth;
	// Event Used for BP song assignment
	UPROPERTY(BlueprintAssignable, BlueprintReadWrite) FOnEntityDie onEntityDie;

	// No need for BP assignment (Use onAttack instead cause no enemy can be in range of attack)
	// Used to prepare which enemys will be hit for the next attack.
	UPROPERTY() FOnPrepareAttack onPrepareAttack;

private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* boxCollision = nullptr;
	
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* spriteComponent = nullptr;
	
protected:
	UPROPERTY(EditAnywhere, Category = "Entity|Fonctionnality")	TArray<TEnumAsByte<EObjectTypeQuery>> attackLayer;
	UPROPERTY(EditAnywhere, Category = "Entity|Fonctionnality")	FVector forwardDirection = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Entity|Fonctionnality") UTexture2D* idleTexture = nullptr;

	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* walkSprite = nullptr;
	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* forswingSprite = nullptr;
	UPROPERTY(EditAnywhere, Category = "Entity|Animation") UPaperSprite* backswingSprite = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") EAttackType attackType = EAttackType::SINGLE;
	// MoveSpeed multiplyed by 3 from the real game.
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float moveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackSpeed = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackForswingTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") float attackBackswingTime = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int attackDamage = 0;
	// AttackRange divided by 4 from the real game.
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int attackRange = 0;
	UPROPERTY(EditAnywhere, Category = "Entity|Stats") int healthPoint = 0;

	//TODO kb, cooldown, %multiplier, win/death, LongRange/OmniStrike, 
	//TODO Boss KB, AI spawn Base Health, catcannon, map, save, Effects, Delayed Damage, 

	UPROPERTY() bool isDead = false;
	UPROPERTY() bool isAttacking = false;
	UPROPERTY() bool hasFinishAttackAnimation = true;
	UPROPERTY() float currentAttackSpeedTimer = 0.0f;
	UPROPERTY() float currentAttackSwingTimer = 0.0f;
	UPROPERTY() EAttackState attackState = EAttackState::NONE;

	// Stock le résultat du LineTrace lors de la détection pour éviter dans refaire le même Tick.
	UPROPERTY() FHitResult hitSingleResult;
	UPROPERTY() TArray<FHitResult> hitMultiResult;

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
	UFUNCTION() virtual void Attack_Single();
	UFUNCTION() virtual void Attack_Area();

	// Fonctionnement de l'attack (Déroulement de l'animation, temps d'attente et appel de la fonction Attack)
	void Attacking(float _deltaTime);
	
public:

	// Fonction virtual pour que les class filles puissent redéfinir son fonctionnement
	// Permet de faire perdre des points de vie à l'unité et la détruire lors de la mort.
	virtual void LoseHealth(int _damageAmount);

private:
	// Permet de calculer la hauteur à laquelle le Sprite se positionne (Car toutes les entitées sont en (0, x, 0))
	void InitSpriteLocation();

	void InitOptimisedEventsCalls();

	UFUNCTION() void INTERNAL_PrepareAttack_Single();
	UFUNCTION() void INTERNAL_PrepareAttack_Area();
};