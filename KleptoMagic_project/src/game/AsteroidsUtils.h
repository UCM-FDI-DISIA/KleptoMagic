//#pragma once
//
//#include "AsteroidsFacade.h"
//#include "../../src/utils/Singleton.h"
//
//using namespace ecs;
//
//class AsteroidsUtils : virtual public AsteroidsFacade, public Singleton<AsteroidsUtils> {
//	friend Singleton<AsteroidsUtils>;
//private:
//	AsteroidsUtils();
//	bool init(Manager* mngr);
//	virtual ~AsteroidsUtils();
//	Manager* _mngr;
//
//public:
//	void create_asteroids(int n);
//	void create_asteroids(int n, int g);
//	void remove_all_asteroids();
//	void split_asteroid(Entity* a);
//	int count_asteroids();
//};
//
//// This macro defines a compact way for using the singleton AsteroidsUtils, instead of
//// writing AsteroidsUtils::instance()->method() we write asteroidsutils().method()
////
//inline AsteroidsUtils& asteroidsutils() {
//	return *AsteroidsUtils::Instance();
//}
//
//