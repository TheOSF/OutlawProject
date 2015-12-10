#pragma once


#include "../Ball/Ball.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"
#include "../Render/MeshRenderer.h"
#include "../GameSystem/GameObject.h"

//***************************************************
//		�x���炵�p�{�[���N���X
//***************************************************

class RainbowSkillBall :public GameObjectBase
{
public:
    //�R���X�g���N
    RainbowSkillBall(CrVector3 pos, CrVector3 move, CharacterBase* target);
    ~RainbowSkillBall();

    bool Update();
    bool Msg(MsgType mt);

private:
    MeshRenderer		m_MeshRenderer;
    LocusHDR            m_Locus;
    bool                m_LiveFlg;
    CharacterBase*const m_Target;
    Vector3             m_Pos, m_Move;

};