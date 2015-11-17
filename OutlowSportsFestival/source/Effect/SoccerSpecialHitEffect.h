#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterBase.h"
#include "../Render/BlurObject.h"
#include "../Damage/Damage.h"
#include "TornadoEffect.h"

//------------------------------------------------//
//   �T�b�J�[�̕K�E�Z���q�b�g�����Ƃ��̃N���X
//------------------------------------------------//

class SoccerSpecialHit :public GameObjectBase
{
public:

    SoccerSpecialHit(
        CharacterBase* pOwner,//�e�L�����N�^�ւ̃|�C���^
        CrVector3      pos,   //�o�����W
        RATIO          level, //�G�t�F�N�g�̃N�I���e�B(�O�`�P)
        UINT           time
        );

    ~SoccerSpecialHit();


private:
    CharacterBase*const  m_pOwner;
    void(SoccerSpecialHit::*m_pStateFunc)();

    TornadoEffect*       m_pTornadoEffect;

    const Vector3        m_Pos;
    const RATIO          m_Level;
                         
    const int            m_LiveTime;
    int                  m_Count;

    DamageCapsure         m_Damage;

    SoccerSpecialHit(const SoccerSpecialHit&);

    bool Update();
    bool Msg(MsgType mt);

    void EffectApper(int n,RATIO scale);
    void Particle(int n);

    void State_Init();
    void State_BlurToSmal();
    void State_Impact();
    void State_Finish();
};
