
#include "StatusEffect.h"

StatusEffect:: StatusEffect(int timer,int strenght,int effectTypenumber)
{
	timeLeft = timer;
	effectStrength = strenght;
	effectnumber = effectTypenumber;
}
int StatusEffect::GetType()
{
	return effectnumber;
}
void StatusEffect:: DecreaseTime()
{
	timeLeft--;
	if (timeLeft <= 0)
	{
		
	}
}
StatusEffect ::~StatusEffect()
{

}
