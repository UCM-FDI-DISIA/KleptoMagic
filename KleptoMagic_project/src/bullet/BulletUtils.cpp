#include "BulletUtils.h"
#include <cmath>
#include "../render/Camera.h"
#include "../bullet/UndeadBulletCollisionComponent.h"

BulletUtils::BulletUtils()
{
	_tim = new VirtualTimer();
	for(int i=0;i<componentes.size();i++)
	{
		componentes[i] = false;
	}
	player = game().getSelectedCharacter();
	bulStat = new BulletStats();
	attSpeedCapFlat = bulStat->Created(player);
	
}

BulletUtils::~BulletUtils()
{
	
}

void BulletUtils::update()
{
	auto* mngr = game().getMngr();
	for (auto bull : mngr->getEntities(ecs::grp::BULLET))
	{
		auto bullStat = mngr->getComponent<BulletStats>(bull);
		if (!bullStat->getPiercing() && _dungeonfloor != nullptr)
		{
			auto tilecollision = mngr->getComponent<TileCollisionChecker>(bull);
			if (!tilecollision->getCanMoveX() || !tilecollision->getCanMoveY()) { mngr->setAlive(bull, false); explode(bull); }
		}
		if(sdlutils().currRealTime() - bullStat->getSartTime()>bullStat->getDuration())
		{
			explode(bull);
			mngr->setAlive(bull, false);
		}
	}
}

void BulletUtils::render()
{
}

void BulletUtils::reset()
{
	auto* mngr = game().getMngr();
	for (auto bull : mngr->getEntities(ecs::grp::BULLET))
	{
		mngr->setAlive(bull, false);
	}
}

void BulletUtils::hit(Entity* targ,Entity* bullet) {
	auto* _mngr = game().getMngr();
	auto* enemyhitted = _mngr->getComponent<EnemyHitted>(bullet);
	if (enemyhitted->AddEnemy(targ)) {
		EntityStat* _sts = _mngr->getComponent<EntityStat>(targ);
		_sts->ChangeStat(-1 * bulStat->getDamage(), EntityStat::Stat::HealthCurrent);
		auto* effects = _mngr->getComponent<StatusEffect>(targ);
		effects->AddStatus(StatusEffect::slow, bulStat->getSlowS(), bulStat->getSlowT());
		effects->AddStatus(StatusEffect::dot, bulStat->getDotS(), bulStat->getDotT());
		effects->AddStatus(StatusEffect::stun,0 , bulStat->getStun());
	}
	auto* stat = _mngr->getComponent<BulletStats>(bullet);
	if (!stat->getPiercing()) { explode(bullet); }
}

void BulletUtils::pressed()
{
	if (_tim->currRealTime() > attSpeedCapFlat*attSpeedCapMul) {
		shoot();
		// Reproducir sonido de ataque
		playerutils().playAttackSound();
	}
}

void BulletUtils::enemyShoot(Transform* _enemyTR, int i)
{
	auto* _mngr = game().getMngr();
	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	
	
	auto _bullets = _mngr->addEntity(ecs::grp::ENEMYBULLET);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->enemyStats(i);
	Vector2D vel = Vector2D(_tr->getPos() - _enemyTR->getPos()).normalize()*stats->getSpeed();
	float rot = -vel.normalize().angle(Vector2D(0, -1));
	auto* _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(_enemyTR->getPos().getX() + _enemyTR->getWidth() / 2, _enemyTR->getPos().getY() + _enemyTR->getHeight() / 2) - Vector2D(stats->getSize() / 2, stats->getSize() / 2), vel, stats->getSize(), stats->getSize(), rot);
	//la imagen debe de ser distinta para cada enemigo
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1,0);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	if (!stats->getPiercing())
	{
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}

}

void BulletUtils::undeadArcherShoot(Transform* _enemyTR, int i) {
	auto* _mngr = game().getMngr();
	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));

	// Crear entidad para la bala
	auto _bullets = _mngr->addEntity(ecs::grp::ENEMYBULLET);

	// Añadir y configurar los stats de la bala
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->enemyStats(i);
	stats->setArcherDamage(2);

	// Reducción del tamaño de la bala
	int originalSize = stats->getSize();
	int reducedSize = originalSize / 4;

	// Calcular la dirección y la velocidad de la bala hacia el jugador
	Vector2D vel = Vector2D(_tr->getPos() - _enemyTR->getPos()).normalize() * stats->getSpeed();
	float rot = -vel.normalize().angle(Vector2D(0, -1));

	// Añadir el componente Transform y posicionar la bala
	auto* _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(
		Vector2D(_enemyTR->getPos().getX() + _enemyTR->getWidth() / 2,
			_enemyTR->getPos().getY() + _enemyTR->getHeight() / 2) -
		Vector2D(reducedSize / 2, reducedSize / 2),
		vel,
		reducedSize,
		reducedSize,
		rot
	);

	// Añadir la imagen para la bala (si es necesario)
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1, 0);

	// Añadir el componente DestroyOnBorder para que la bala se elimine si sale del mapa
	_mngr->addComponent<DestroyOnBorder>(_bullets);

	// Añadir el componente de colisión si la bala no tiene efecto de perforación
	if (!stats->getPiercing()) {
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}

	// **Añadir el componente de colisión personalizado para esta bala**
	_mngr->addComponent<UndeadBulletCollisionComponent>(_bullets);
}


void BulletUtils::BossManyDirectinons(Transform* bossTR, Vector2D v)
{
	BulletStats* stat = new BulletStats();
	stat->enemyStats(2);
	MultiShot(v, stat, false,bossTR);
}

void BulletUtils::collided(ecs::entity_t e)
{
	auto* mngr = game().getMngr();
	auto bullStat = mngr->getComponent<BulletStats>(e);
	if(!bullStat->getPiercing())
	{
		mngr->setAlive(e, false);
	}
	explode(e);
}

void BulletUtils::addComponent(int i)
{
	componentes[i] = true;
}

void BulletUtils::checkComponent(int i, ecs::entity_t bullet)
{
	auto* _mngr = game().getMngr();
	if(i==0)
	{
		_mngr->addComponent<HomingComponent>(bullet);
	}
}



void BulletUtils::explode(ecs::entity_t bullet)
{
	
	auto* _mngr = game().getMngr();
	auto* _bstat = _mngr->getComponent<BulletStats>(bullet);
	if (_bstat->getExplode()) {
		auto* _tr = _mngr->getComponent<Transform>(bullet);
		auto _bullets = _mngr->addEntity(_mngr->groupId(bullet));
		auto* stats = _mngr->addComponent<BulletStats>(_bullets);
		stats->explosionStats(_bstat->getDamage());
		auto _bulletsTR = _mngr->addComponent<Transform>(_bullets);
		_bulletsTR->init(Vector2D(_tr->getPos().getX() + _tr->getWidth() / 2, _tr->getPos().getY() + _tr->getHeight() / 2) - Vector2D(stats->getSize() / 2, stats->getSize() / 2), { 0,0 }, stats->getSize(), stats->getSize(), 0);
		_mngr->addComponent<ImageWithFrames>(_bullets, explosion, 1, 1, 0);
		_mngr->addComponent<EnemyHitted>(_bullets);
	}
}

void BulletUtils::shoot()
{
	auto* _mngr = game().getMngr();
	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	Vector2D playerCenter = {_tr->getPos().getX() + _tr->getWidth() / 2,_tr->getPos().getY() + _tr->getHeight() / 2	};
	Vector2D aim = input().getAimVector(playerCenter - camOffset);
	if (bulStat->getBull() > 1) {
		MultiShot(aim,bulStat,true);
	}
	else {
		IndividualShotP(aim);
	}
 	_tim->resetTime();

}

void BulletUtils::MultiShot(Vector2D v, BulletStats* stat, bool fromPlayer,Transform* tr)
{
	float fullAngle = 15 + 5 * (stat->getBull() - 2);
	float moveAngle = fullAngle / (stat->getBull()-1);
	float initialAngle = atan2(-v.getY(), v.getX()) * 180 / M_PI;
	initialAngle =initialAngle- (fullAngle / 2);
	float dirInRad;
	for (int i = 0; i < stat->getBull(); i++) {
		
		dirInRad = (initialAngle+moveAngle*i) * (M_PI / 180);
		Vector2D dir = { cos(dirInRad),- sin(dirInRad) };
		if (fromPlayer) {
			IndividualShotP(dir.normalize());
		}
		else
		{
			IndividualShotH(dir.normalize(), tr);
		}
		
	}
}

void BulletUtils::IndividualShotP(Vector2D v)
{
	auto* _mngr = game().getMngr();

	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));
	int x, y;
	SDL_GetMouseState(&x, &y);
	float xf = static_cast<float>(x);
	float yf = static_cast<float>(y);
	Vector2D PosRat = { xf,yf };

	if (input().isControllerConnected()) {
		input().triggerRumble(RumbleType::TAP);
	}

	auto _bullets = _mngr->addEntity(ecs::grp::BULLET);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->refreshStats(bulStat->getSpeed(), bulStat->getDamage(), bulStat->getDistance(), bulStat->getSize(), bulStat->getPiercing(),bulStat->getExplode(), bulStat->getBull(), bulStat->getSlowT(), bulStat->getSlowS(), bulStat->getStun(), bulStat->getDotT(), bulStat->getDotS());
	Vector2D vel = v * stats->getSpeed();
	float rot = atan2(vel.getY(), vel.getX()) * 180.0f / M_PI ;
	auto _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(_tr->getPos().getX() + _tr->getWidth() / 2, _tr->getPos().getY() + _tr->getHeight() / 2) - Vector2D(stats->getSize() / 2, stats->getSize() / 2), vel, stats->getSize(), stats->getSize(), rot);
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1, 0);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	_mngr->addComponent<EnemyHitted>(_bullets);
	//_mngr->addComponent<>
	for (int i = 0; i < componentes.size(); i++)
	{
		if (componentes[i]) { checkComponent(i, _bullets); }
	}
	if (!bulStat->getPiercing())
	{
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}

}

void BulletUtils::IndividualShotH(Vector2D v, Transform* tr)
{
	auto* _mngr = game().getMngr();
	Transform* _tr = _mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::PLAYER));


	auto _bullets = _mngr->addEntity(ecs::grp::ENEMYBULLET);
	auto* stats = _mngr->addComponent<BulletStats>(_bullets);
	stats->enemyStats(2);
	Vector2D vel = Vector2D(_tr->getPos() - tr->getPos()).normalize() * stats->getSpeed();
	float rot = -vel.normalize().angle(Vector2D(0, -1));
	auto* _bulletsTR = _mngr->addComponent<Transform>(_bullets);
	_bulletsTR->init(Vector2D(tr->getPos().getX() + tr->getWidth() / 2, tr->getPos().getY() + tr->getHeight() / 2) - Vector2D(stats->getSize() / 2, stats->getSize() / 2), vel, stats->getSize(), stats->getSize(), rot);
	//la imagen debe de ser distinta para cada enemigo
	_mngr->addComponent<ImageWithFrames>(_bullets, tex, 1, 1, 0);
	_mngr->addComponent<DestroyOnBorder>(_bullets);
	_mngr->addComponent<HomingComponent>(_bullets);
	if (!stats->getPiercing())
	{
		auto tilechecker = _mngr->addComponent<TileCollisionChecker>(_bullets);
		tilechecker->init(true, _bulletsTR, _dungeonfloor);
		_bulletsTR->initTileChecker(tilechecker);
	}
}
