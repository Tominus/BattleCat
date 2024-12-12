#include "BC_Entity.h"

#include "BC_FightGameMode.h"
#include "PaperSprite.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


ABC_Entity::ABC_Entity()
{
	PrimaryActorTick.bCanEverTick = true;

	boxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	spriteComponent = CreateDefaultSubobject<UPaperSpriteComponent>("SpriteComponent");

	spriteComponent->SetupAttachment(boxCollision);
	spriteComponent->AddLocalRotation(FRotator(0, 90, 0));
	spriteComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = boxCollision;
}

void ABC_Entity::BeginPlay()
{
	Super::BeginPlay();
	InitSpriteLocation();
	InitOptimisedEventsCalls();

	//Can directly attack when spawned
	currentAttackSpeedTimer = attackSpeed;
}

void ABC_Entity::DetectEnemy()
{
	if (!hasFinishAttackAnimation)return;
	
	const FVector& _startLocation = GetActorLocation();
	const FVector& _endLocation = _startLocation + forwardDirection * attackRange;
	
	FHitResult _hitResult;
	bool _hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
																   _startLocation,
																   _endLocation,
																   attackLayer,
																   false,
																   TArray<AActor*>(),
																   EDrawDebugTrace::ForOneFrame,
																   _hitResult,
																   true);

	// L'entitée reste en phase d'attack lorsqu'elle détecte un enemy ou qu'elle n'a pas finit son animation d'attack.
	isAttacking = _hasHit || !hasFinishAttackAnimation;
}

void ABC_Entity::Move(float _deltaTime)
{
	if (isAttacking)return;

	// Déplace l'entité dans la direction voulue.
	AddActorWorldOffset(forwardDirection * _deltaTime * moveSpeed);
}

void ABC_Entity::Attack_Single()
{
	// Declaration in childs class
}

void ABC_Entity::Attack_Area()
{
	// Declaration in childs class
}

void ABC_Entity::Attacking(float _deltaTime)
{
	// Update sans cesse le timer d'attack car peu importe ce qu'il se passe, passé un certain delay (attackSpeed),
	// mon unité va pouvoir redenouveau utiliser son attack.
	currentAttackSpeedTimer += _deltaTime;
	
	if (!isAttacking)
	{
		// Ici ma phase d'attack est finie, alors je remet le sprite de marche.
		attackState = EAttackState::NONE;
		spriteComponent->SetSprite(walkSprite);
		return;
	}

	if (attackState == EAttackState::NONE)
	{
		// Timer global d'attack, une fois atteint (lorsque notre variable isAttacking devient true)
		// Notre unitée passe en début de phase d'attack (Forswing)
		// A ce moment, mon unité sera obligé de finir toute son animation si je veux faire autre chose.
		if (currentAttackSpeedTimer >= attackSpeed)
		{
			attackState = EAttackState::FORSWING;
			currentAttackSwingTimer = 0.0f;
			spriteComponent->SetSprite(forswingSprite);
			hasFinishAttackAnimation = false;
		}
	}
	else if (attackState == EAttackState::FORSWING)
	{
		currentAttackSwingTimer += _deltaTime;

		// Une fois passé le delay de début d'attack, j'appelle la fonction Attack (définie par les class filles)
		// et je passe en mode fin d'attaque. (Backswing)
		// Reset le timer d'attack Global.
		if (currentAttackSwingTimer >= attackForswingTime)
		{
			attackState = EAttackState::BACKSWING;
			currentAttackSpeedTimer = 0.0f;
			currentAttackSwingTimer = 0.0f;

			onPrepareAttack.Broadcast();
			onAttack.Broadcast();
			
			spriteComponent->SetSprite(backswingSprite);
		}
	}
	else if (attackState == EAttackState::BACKSWING)
	{
		currentAttackSwingTimer += _deltaTime;

		// Même timer qu'au dessus sauf qu'une fois atteint, mon entité finit sa phase d'attaque.
		if (currentAttackSwingTimer >= attackBackswingTime)
		{
			attackState = EAttackState::NONE;
			currentAttackSwingTimer = 0.0f;
			spriteComponent->SetSprite(walkSprite);
			hasFinishAttackAnimation = true;
		}
	}
}

void ABC_Entity::LoseHealth(int _damageAmount, const TArray<ETraitType>& _attackerTraitsType)
{
	healthPoint -= _damageAmount;

	onLoseHealth.Broadcast();

	if (healthPoint <= 0)
	{
		healthPoint = 0;
		isDead = true;
		onEntityDie.Broadcast();
		Destroy();
	}
}

void ABC_Entity::InitSpriteLocation()
{
	RETURN_LOG(!idleTexture, "ABC_Entity::InitSpriteLocation -> Missing Walk Texture");
	
	// Prend la moitié de la taille de la texture et la rajoute au component,
	// pour donner l'impression que l'Entity est sur le sol.
	
	spriteComponent->SetRelativeLocation(FVector(0, 0, idleTexture->GetSizeY() / 2.0f));
}

void ABC_Entity::InitOptimisedEventsCalls()
{
	if (attackType == EAttackType::SINGLE)
	{
		onPrepareAttack.AddUniqueDynamic(this, &ABC_Entity::INTERNAL_PrepareAttack_Single);
		onAttack.AddUniqueDynamic(this, &ABC_Entity::Attack_Single);
	}
	else if (attackType == EAttackType::AREA)
	{
		onPrepareAttack.AddUniqueDynamic(this, &ABC_Entity::INTERNAL_PrepareAttack_Area);
		onAttack.AddUniqueDynamic(this, &ABC_Entity::Attack_Area);
	}
}

void ABC_Entity::INTERNAL_PrepareAttack_Single()
{
	const FVector& _startLocation = GetActorLocation();
	const FVector& _endLocation = _startLocation + forwardDirection * attackRange;
	
	UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
												    _startLocation,
												    _endLocation,
												    attackLayer,
												    false,
												    TArray<AActor*>(),
												    EDrawDebugTrace::ForOneFrame,
												    hitSingleResult,
												    true);
}

void ABC_Entity::INTERNAL_PrepareAttack_Area()
{
	const FVector& _startLocation = GetActorLocation();
	const FVector& _endLocation = _startLocation + forwardDirection * attackRange;

	UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(),
												   _startLocation,
												   _endLocation,
												   attackLayer,
												   false,
												   TArray<AActor*>(),
												   EDrawDebugTrace::ForOneFrame,
												   hitMultiResult,
												   true);
}