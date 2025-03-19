
#include "StatusEffect.h"

StatusEffect:: StatusEffect(int timer,int strenght,int effectTypenumber)
{
	timeLeft = timer;
	effectStrength = strenght;
}
//int StatusEffect::GetType()
//{
//	return effectnumber;
//}
//int StatusEffect::DecreaseTime()
//{
//	timeLeft--;
//	if (timeLeft <= 0)
//	{
//		return 0;
//	}
//	return timeLeft;
//}
StatusEffect ::~StatusEffect()
{

}
