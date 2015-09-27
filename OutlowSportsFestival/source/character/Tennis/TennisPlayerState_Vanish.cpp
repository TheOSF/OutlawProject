#include "TennisPlayerState_Vanish.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"


TennisState_DamageVanish::TennisState_DamageVanish(
    TennisPlayer* pTennis,
    const Vector3& Damage_vec  //�_���[�W���󂯂�����
    ) :
    m_pTennis(pTennis),
    m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageVanish::Enter(TennisPlayer* t)
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
            //�N���オ�胂�[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_StandUp);
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
            //�ʏ�X�e�[�g���Z�b�g
            m_pTennis->SetState(
                TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
                );
        }


    private:
        TennisPlayer*  m_pTennis;
    };

    //�_���[�W���[�V�����p�����[�^�[���쐬����
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = Vector3Normalize(m_Damage_vec)*0.6f;
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
        new TennisHitEvent(t)
        );

    //�q�b�g�G�t�F�N�g�쐬
    new HitEffectObject(
        m_pTennis->m_Params.pos + Vector3(0, 3, 0),
        m_Damage_vec,
        0.05f,
        0.15f,
        Vector3(1.0f, 1.0f, 1.0f)
        );

    //�u���[�G�t�F�N�g
    new BlurImpactSphere(
        m_pTennis->m_Params.pos + Vector3(0, 3, 0),
        25,
        10,
        30
        );
}

void TennisState_DamageVanish::Execute(TennisPlayer* t)
{
    m_pDamageVanishClass->Update();
}

void TennisState_DamageVanish::Exit(TennisPlayer* t)
{
    delete m_pDamageVanishClass;
}