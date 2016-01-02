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

    TornadoEffect*              m_pTornadoEffect;

    const Vector3               m_Pos;
    const RATIO                 m_Level;
                                
    const int                   m_LiveTime;
    int                         m_Count;

    DamageControllVanish*       m_pDamage;

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


//-----------------------------------------------------//
//   �T�b�J�[�̕K�E�Z���q�b�g�����Ƃ�������ё���N���X
//-----------------------------------------------------//


class TornadoVanishControll :public  GameObjectBase
{
public:
    struct InitParam
    {
        Vector3 from;

        RADIAN  start_angle_seed;
        RADIAN  all_rotation_angle;
        
        float   start_width;
        float   end_width;

        float   height;

        RATIO   speed;

        float   damage;
    };

    TornadoVanishControll(
        const InitParam& param,
        DamageControll_Transform* pControll_Transform);

    ~TornadoVanishControll();

    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt);

private:
    const InitParam                 m_Param;
    DamageControll_Transform* const m_pControll_Transform;
    RATIO                           m_Count;
    int                             m_DamageCount;

    //�ړ��X�V
    bool Move();

    //�p���s����X�V
    void SetTransform();

    //�_���[�W�����Z
    void AddDamage(float value, bool dontDie);
};