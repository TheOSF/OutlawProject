#include "SoccerComputerUtilityClass.h"

Vector3 SocceComputerrUtillityClass::ComputerRollingControll::GetVec()
{
	float v = PI / (1 + rand() % 4);
	Vector3 vec(stick.x+v, 0, stick.z-v);

	return vec;
}