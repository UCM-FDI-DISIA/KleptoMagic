//#pragma once
//
//#include "FighterFacade.h"
//#include "../../src/utils/Singleton.h"
//
//using namespace ecs;
//
//class FighterUtils : virtual public FighterFacade, public Singleton<FighterUtils> {
//	friend Singleton<FighterUtils>;
//private:
//	FighterUtils();
//	bool init(Manager* mngr);
//	virtual ~FighterUtils();
//	Manager* _mngr;
//
//public:
//	void create_fighter();
//	void reset_fighter();
//	void reset_lives();
//	void take_life();
//	int get_lives();
//	void update_lives(int n);
//};
//
//// This macro defines a compact way for using the singleton AsteroidsUtils, instead of
//// writing FighterUtils::instance()->method() we write fighterutils().method()
////
//inline FighterUtils& fighterutils() {
//	return *FighterUtils::Instance();
//}
//
//