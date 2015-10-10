#include "TennisPlayerState_DamageMotionDie.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/BlurImpact.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Ball/Ball.h"

#include "../../Camera/Camera.h"

TennisState_DamageMotion_Die::TennisState_DamageMotion_Die(
    TennisPlayer* pTennis,
    const Vector3& Damage_vec  //�_���[�W���󂯂�����
    ) :
    m_pTennis(pTennis),
    m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageMotion_Die::Enter(TennisPlayer* t)
{
    //�L�����N�^���ʂЂ�݃N���X�̃e�j�X�ŗL�C�x���g�N���X
    class TennisEvent :public CharacterDamageVanish::Event
    {
    public:
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis){}

        void FlyStart()
        {
            //������у��[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);
            m_pTennis->m_Params.camera_draw = false;
        }

        void Flying(const Matrix& Rotate)
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);

            //������т̉�]������
            m_pTennis->m_Renderer.m_TransMatrix = Rotate*m_pTennis->m_Renderer.m_TransMatrix;
        }

        void DownStart()
        {
            //�_�E�����[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Down);
        }

        void Downing()
        {
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);

        }

        void StandUpStart()
        {
            //�������Ȃ�
        }

        void StandUping()
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void End()
        {
            //�J�����ʂ��t���O��false��
            m_pTennis->m_Params.camera_draw = false;
        }

    private:
        TennisPlayer*  m_pTennis;
    };

    //�_���[�W���[�V�����p�����[�^�[���쐬����
    CharacterDamageVanish::Param  Param;

    m_Damage_vec.y = 0;
    m_Damage_vec.Normalize();

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0f);

    Param.move = m_Damage_vec * 0.7f;
    Param.move.y = 0.2f;

    Param.down_frame = 15;
    Param.down_muteki_frame = 15;
    Param.standup_frame = 50;
    Param.standup_muteki_frame = 10;

    //�Ђ�݃N���X���쐬
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t),
        new DamageManager::HitEventBase()
        );
    
    {
        COLORf EffectColor(CharacterBase::GetPlayerColor(t->m_PlayerInfo.number));
 
        //�q�b�g�G�t�F�N�g�쐬
        new HitEffectObject(
            m_pTennis->m_Params.pos + Vector3(0, 3, 0) + m_Damage_vec*3.5f,
            m_Damage_vec,
            0.05f,
            0.15f,
            Vector3(EffectColor.r, EffectColor.g, EffectColor.b),
            5,
            50
            );
    }

    ////�u���[�G�t�F�N�g
    //new BlurImpactSphere(
    //    m_pTennis->m_Params.pos + Vector3(0, BallBase::UsualBallShotY, 0),
    //    20,
    //    50,
    //    15
    //    );

    //�J�����V���b�N
    DefCamera.SetShock(
        Vector2(1, 1)*0.22f, 
        20
        );
}

void TennisState_DamageMotion_Die::Execute(TennisPlayer* t)
{
    m_pDamageVanishClass->Update();
}

void TennisState_DamageMotion_Die::Exit(TennisPlayer* t)
{
    delete m_pDamageVanishClass;
}