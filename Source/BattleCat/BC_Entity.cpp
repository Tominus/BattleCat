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

	//Can directly attack when spawned
	currentAttackSpeedTimer = attackSpeed;
}

void ABC_Entity::DetectEnemy()
{
	bool _hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),
																   GetActorLocation(),
																   GetActorLocation() + forwardDirection * attackRange,
																   attackLayer,
																   false,
																   TArray<AActor*>(),
																   EDrawDebugTrace::ForOneFrame,
																   hitResult,
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

void ABC_Entity::Attack()
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
			
			Attack();
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

void ABC_Entity::LoseHealth(int _damageAmount)
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
	UPaperSprite* _paperSprite = spriteComponent->GetSprite();
	if (!_paperSprite)return;

	// Prend la moitié de la taille du sprite et le rajoute au component,
	// pour donner l'impression que l'Entity est sur le sol.
	spriteComponent->SetRelativeLocation(FVector(0, 0, _paperSprite->GetSourceSize().Y / 2.0f));
}