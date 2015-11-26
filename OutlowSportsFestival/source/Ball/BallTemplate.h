#pragma once


#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"
#include "../Render/LightObject.h"

class BallTemplate :public GameObjectBase, public BallBase
{
public:
    
    CharacterBase* const    m_pFirstParent;

private:

};