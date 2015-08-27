#include "TennisPlayerState_DamageMotionDie.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"

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
        TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis), m_Timer(0){}

        void Flying(const Matrix& Rotate)
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //�ʒu�ɂ��Ƃ����A���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);

            //������т̉�]������
            m_pTennis->m_Renderer.m_TransMatrix = Rotate*m_pTennis->m_Renderer.m_TransMatrix;
        }

        void StandUpStart()
        {
            //������у��[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Down);
        }

        void StandUping()
        {
            //���f���̃A�j���[�V�����X�V
            m_pTennis->m_Renderer.Update(1);

            //���[���h�ϊ��s����v�Z
            chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
        }

        void Start()
        {
            //������у��[�V�������Z�b�g
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Vanish_Fly);

        }

        void End()
        {

        }
    private:
        TennisPlayer*  m_pTennis;
        int            m_Timer;
    };

    //�_���[�W���[�V�����p�����[�^�[���쐬����
    CharacterDamageVanish::Param Param;

    Param.rotate_speed = Vector3(0.0f, 0.0f, 0.0);
    Param.move = m_Damage_vec * 0.9f;
    Param.move.y = 0.2f;
    Param.standup_frame = 30;

    //�Ђ�݃N���X���쐬
    m_pDamageVanishClass = new CharacterDamageVanish(
        m_pTennis,
        Param,
        new TennisEvent(t)
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