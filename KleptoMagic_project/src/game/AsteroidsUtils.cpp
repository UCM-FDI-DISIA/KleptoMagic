#include "AsteroidsUtils.h"

#include "../components/Image.h"
#include "../components/Transform.h"
#include "../components/ShowAtOppositeSide.h"
#include "../components/Follow.h"
#include "../components/Generations.h"
#include "../components/TowardsDestination.h"
#include "../components/ImageWithFrames.h"
#include "../sdlutils/SDLUtils.h"
#include "../utils/Vector2D.h"

AsteroidsUtils::AsteroidsUtils() :
	_mngr(nullptr)
{
}
AsteroidsUtils::~AsteroidsUtils() {
	remove_all_asteroids();
}

bool AsteroidsUtils::init(Manager* mngr) {
	_mngr = mngr;
	assert(_mngr != nullptr);
	return true;
}

void AsteroidsUtils::create_asteroids(int n) {
	for (int i = 0; i < n; i++) {
		auto* asteroid = _mngr->addEntity(ecs::grp::ASTEROIDS);
		auto tr = _mngr->addComponent<Transform>(asteroid);
		auto g = sdlutils().rand().nextInt(1, 3);
		auto s = 10.0f + 5.0f * g;
		auto x = sdlutils().rand().nextInt(0, sdlutils().width());
		auto y = sdlutils().rand().nextInt(0, sdlutils().height());
		tr->init(Vector2D(x, y), Vector2D(1, 1), s, s, 0.0f);
		_mngr->addComponent<ImageWithFrames>(asteroid, &sdlutils().images().at("asteroid"), 6, 5);
		_mngr->addComponent<ShowAtOppositeSide>(asteroid);
		auto gen = _mngr->addComponent<Generations>(asteroid);
		gen->setGen(g);
		if (sdlutils().rand().nextInt(1, 10) == 6) {
			_mngr->addComponent<Follow>(asteroid);
		}
		else {
			_mngr->addComponent<TowardsDestination>(asteroid);
		}
	}
}

void AsteroidsUtils::remove_all_asteroids() {
	for (auto e : _mngr->getEntities(ecs::grp::ASTEROIDS)) {
		_mngr->setAlive(e, false);
	}
}

void AsteroidsUtils::split_asteroid(Entity* a) {
	auto old_gen = _mngr->getComponent<Generations>(a)->getGen();

	if (old_gen > 0) {
		auto old_p = _mngr->getComponent<Transform>(a)->getPos();
		_mngr->setAlive(a, false);
		for (int i = 0; i < 2; i++) {
			auto* asteroid = _mngr->addEntity(ecs::grp::ASTEROIDS);
			auto tr = _mngr->addComponent<Transform>(asteroid);
			auto g = old_gen - 1;
			auto s = 10.0f + 5.0f * g;
			auto x = old_p.getX();
			auto y = old_p.getY();
			tr->init(Vector2D(x, y), Vector2D(1, 1), s, s, 0.0f);
			_mngr->addComponent<ImageWithFrames>(asteroid, &sdlutils().images().at("asteroid"), 6, 5);
			_mngr->addComponent<ShowAtOppositeSide>(asteroid);
			auto gen = _mngr->addComponent<Generations>(asteroid);
			gen->setGen(g);
			if (sdlutils().rand().nextInt(1, 10) == 6) {
				_mngr->addComponent<Follow>(asteroid);
			}
			else {
				_mngr->addComponent<TowardsDestination>(asteroid);
			}
		}
	}
	_mngr->setAlive(a, false);
}

int AsteroidsUtils::count_asteroids() {
	int result = 0;
	for (auto e : _mngr->getEntities(ecs::grp::ASTEROIDS)) {
		if (_mngr->isAlive(e)) result++;
	}
	return result;
}