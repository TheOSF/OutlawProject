#pragma once

#include "../GameSystem/GameObject.h"
#include "../character/CharacterBase.h"
#include "../Render/BlurObject.h"


//------------------------------------------------//
//   �T�b�J�[�̕K�E�Z���q�b�g�����Ƃ��̃N���X
//------------------------------------------------//

class SoccerSpecialHit :public GameObjectBase
{
public:

    SoccerSpecialHit(
        CharacterBase* pOwner,//�e�L�����N�^�ւ̃|�C���^
        CrVector3      pos,   //�o�����W
        CrVector3      vec,   //����
        RATIO          level  //�G�t�F�N�g�̃N�I���e�B(�O�`�P)
        );

    ~SoccerSpecialHit();


private:
    CharacterBase*const  m_pOwner;
    void(SoccerSpecialHit::*m_pStateFunc)();

    BlurObjectSphere     m_BlurSphere;

    const Vector3        m_Pos;
    const Vector3        m_Vec;
    const RATIO          m_Level;
                         
    int                  m_Count;

    SoccerSpecialHit(const SoccerSpecialHit&);

    bool Update();
    bool Msg(MsgType mt);

    void EffectApper(int n);
    void Particle(int n);

    void State_Init();
    void State_BlurToSmal();
    void State_Impact();
    void State_Finish();
};
