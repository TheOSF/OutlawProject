#include "TennisPlayerState_DamageMotionWeak.h"
#include "TennisPlayerState_UsualMove.h"
#include "Tennis_HitEvent.h"
#include "../CharacterFunction.h"
#include "../../Effect/HitEffectObject.h"
#include "../../Effect/BlurImpact.h"
#include "../../GameSystem/GameController.h"

TennisState_DamageMotion_Weak::TennisState_DamageMotion_Weak(
	TennisPlayer*  pTennis,
	const Vector3& Damage_vec  //�_���[�W���󂯂�����
	):
	m_pTennis(pTennis),
	m_Damage_vec(Damage_vec)
{

}

void TennisState_DamageMotion_Weak::Enter(TennisPlayer* t)
{
	//�L�����N�^���ʂЂ�݃N���X�̃e�j�X�ŗL�C�x���g�N���X
	class TennisEvent :public CharacterDamageMotion::Event
	{
	public:
		TennisEvent(TennisPlayer* pTennis) :m_pTennis(pTennis){}

		void Update(float speed)
		{
			//���f���̍X�V�̂�
			m_pTennis->m_Renderer.Update(1);
			chr_func::CreateTransMatrix(m_pTennis, m_pTennis->m_ModelSize, &m_pTennis->m_Renderer.m_TransMatrix);
		}
		void Start()
		{
			//�Ђ�݃��[�V�������Z�b�g
			m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_Damage_Weak);
		}
		void End()
		{
			//�ʏ�X�e�[�g���Z�b�g
			m_pTennis->SetState(
				TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis)
				);

            m_pTennis->m_Renderer.m_HDR = Vector3Zero;
		}

        void SetLight(float power)
        {
            m_pTennis->m_Renderer.m_HDR = Vector3(1, 1, 1) * power;
        }
	private:
		TennisPlayer*  m_pTennis;
	};
	 
	//�_���[�W���[�V�����p�����[�^�[���쐬����
	CharacterDamageMotion::Params Param;

	Param.damage_vec = m_Damage_vec;

	//�Ђ�݃N���X���쐬
	m_pDamageMotionClass = new CharacterDamageMotion(
		m_pTennis,
		new TennisEvent(m_pTennis),
		new TennisHitEvent(m_pTennis),
		Param
        );

    {
        Vector3 pos, vec;
        
        vec = m_Damage_vec;
        vec.y = 0;
        vec.Normalize();


        pos = m_pTennis->m_Params.pos + Vector3(0, 3.5f, 0) + vec*1.5f;

        //�q�b�g�G�t�F�N�g�쐬
        new HitEffectObject(
            pos,
            vec,
            0.045f,
            0.15f,
            Vector3(1.0f, 0.8f, 0.25f),
            2
            );

    }


    //�R���g���[����U��
    controller::SetVibration(
        5000,
        0.15f,
        m_pTennis->m_PlayerInfo.number
        );

}

void TennisState_DamageMotion_Weak::Execute(TennisPlayer* t)
{
	m_pDamageMotionClass->Update();
}

void TennisState_DamageMotion_Weak::Exit(TennisPlayer* t)
{
    delete m_pDamageMotionClass;
}