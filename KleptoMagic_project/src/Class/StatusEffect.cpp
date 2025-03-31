
#include "StatusEffect.h"

StatusEffect:: StatusEffect()
{
	
}
//int StatusEffect::GetType()
//{
//	return effectnumber;
//}
void StatusEffect::initComponent() { //falta get entity stats speed
	auto* _mngr = _ent->getMngr();
	_entStat = _mngr->getComponent<EntityStat>(_ent);
	assert(_entStat != nullptr);
}

void StatusEffect::update() {
	DecreaseTime();
	UpdateSpeed();
}
void StatusEffect::AddStatus(Status stat, int strength, float time) {
	//ahora se sobrepone en vez de mirar cual es mas potente o dura más pero eso lo cambio luego
	effects[stat] = {time, strength};
}
void StatusEffect::DecreaseTime()
{
	for (auto& [status, info] : effects) {
		if (info.duration > 0) {
			info.duration -= 1; // Reduce la duración en 1
			if (info.duration < 0) {
				info.strength = 0;
			}
		}
	}
}
void StatusEffect::UpdateSpeed() {
	int speedDebuff = 1;

	if (effects[Status::stun].duration > 0) {
		speedDebuff = 0;
	}
	else {
		speedDebuff = 1 - effects[Status::slow].strength;
	}
	_entStat->UpdateDebuff(speedDebuff);
}

StatusEffect ::~StatusEffect()
{

}
