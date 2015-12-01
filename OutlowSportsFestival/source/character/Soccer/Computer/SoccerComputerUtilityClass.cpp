#include "SoccerComputerUtilityClass.h"

Vector3 SocceComputerrUtillityClass::ComputerRollingControll::GetVec()
{

	Vector3 vec(stick.x, 0, stick.z);

	return vec;
}