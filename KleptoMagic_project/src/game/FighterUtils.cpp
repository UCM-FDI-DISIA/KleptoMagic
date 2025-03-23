//#include "FighterUtils.h"
//
//#include "../Class/Image.h"
//#include "../Class/Transform.h"
//#include "../components/FighterCtrl.h"
//#include "../components/DeAcceleration.h"
//#include "../components/ShowAtOppositeSide.h"
//#include "../components/Gun.h"
//#include "../components/Health.h"
//#include "../sdlutils/SDLUtils.h"
//#include "../utils/Vector2D.h"
//
//FighterUtils::FighterUtils() :
//	_mngr(nullptr)
//{
//}
//FighterUtils::~FighterUtils() {
//
//} 
//bool FighterUtils::init(Manager* mngr) {
//	_mngr = mngr;
//	assert(_mngr != nullptr);
//	return true;
//}
//
//
//void FighterUtils::create_fighter() {
//	auto fighter = _mngr->addEntity();
//	_mngr->setHandler(ecs::hdlr::FIGHTER, fighter);
//	auto tr = _mngr->addComponent<Transform>(fighter);
//	auto s = 50.0f;
//	auto x = (sdlutils().width() - s) / 2.0f;
//	auto y = (sdlutils().height() - s) / 2.0f;
//	tr->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
//	_mngr->addComponent<Image>(fighter, &sdlutils().images().at("fighter"));
//	_mngr->addComponent<FighterCtrl>(fighter);
//	_mngr->addComponent<DeAcceleration>(fighter);
//	_mngr->addComponent<ShowAtOppositeSide>(fighter);
//	_mngr->addComponent<Gun>(fighter);
//	_mngr->addComponent<Health>(fighter);
//}
//void FighterUtils::reset_fighter() {
//	_mngr->getComponent<Gun>(_mngr->getHandler(ecs::hdlr::FIGHTER))->reset();
//	auto s = 50.0f;
//	auto x = (sdlutils().width() - s) / 2.0f;
//	auto y = (sdlutils().height() - s) / 2.0f;
//	_mngr->getComponent<Transform>(_mngr->getHandler(ecs::hdlr::FIGHTER))->init(Vector2D(x, y), Vector2D(), s, s, 0.0f);
//}
//void FighterUtils::reset_lives() {
//	_mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::FIGHTER))->resetLives();
//}
//void FighterUtils::take_life() {
//	_mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::FIGHTER))->takeLife();
//}
//int FighterUtils::get_lives() {
//	return _mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::FIGHTER))->getLives();
//}
//void FighterUtils::update_lives(int n) {
//	_mngr->getComponent<Health>(_mngr->getHandler(ecs::hdlr::FIGHTER))->setLives(n);
//}