#include "TennisPlayerState_Counter.h"
#include "TennisPlayerState_UsualMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../../Ball/UsualBall.h"
#include "../../Damage/Damage.h"
#include "Tennis_HitEvent.h"

#include "../CharacterCounterClass.h"
#include "../CharacterMoveClass.h"
#include "../CharacterDefaultCounterClass.h"


//***************************************************
//		�v���C���[����� �J�E���^�[�N���X
//***************************************************

// �X�e�[�g�J�n
void TennisState_PlayerControll_Counter::Enter(TennisPlayer* t)
{
    //�J�E���^�[�C�x���g�N���X
    class CounterEvent :public CharacterDefaultCounter::Event
    {
        TennisPlayer* const m_pTennis;
    public:
        CounterEvent(TennisPlayer* pTennis):
            m_pTennis(pTennis)
        {

        }

        //�\���J�n
        void Pose()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterPose);
        }

        //�{�[���ֈړ��J�n
        void Move(BallBase* pCounterBall)
        {
            //�{�[���̈ʒu�ɂ���ă��[�V��������
            if (chr_func::isRight(m_pTennis, pCounterBall->m_Params.pos))
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterRight);
            }
            else
            {
                m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_CounterLeft);
            }
        }

        //�ł�
        void Shot(BallBase* pCounterBall)
        {
            chr_func::ResetMove(m_pTennis);
            chr_func::AddMoveFront(m_pTennis, -0.15f, 1000);
        }

        //�ł����s
        void ShotFaild()
        {
            m_pTennis->m_Renderer.SetMotion(TennisPlayer::_mt_LosePose);
        }

        //�I��
        void End()
        {
            //�ʏ�ړ��N���X��
            m_pTennis->SetState(TennisState_PlayerControll_Move::GetPlayerControllMove(m_pTennis));
        }
    };

    //�J�E���^�[�p�����[�^�ݒ�
    CharacterDefaultCounter::Param param;

    param.AfterShotFrame = 13;
    param.CanCounterFrame = 30;
    param.CatchAriaSize = 8;
    param.ControllRadian = D3DXToRadian(33);
    param.FailedFrame = 20;
    param.PoseFrame = 24;
    param.ShotFrame = 8;
    param.BallSpeed = 0.85f;

	// �J�E���^�[�N���X�쐬
    m_pCounter = new CharacterDefaultCounter(
        t,
        param,
        new CounterEvent(t),
        new TennisHitEvent(t)
        );
}


// �X�e�[�g���s
void TennisState_PlayerControll_Counter::Execute(TennisPlayer* t)
{
    
    m_pCounter->SetStickValue(controller::GetStickValue(controller::stick::left, t->m_PlayerInfo.number));
	//�X�V
	m_pCounter->Update();

	//���f���̃��[���h�ϊ��s����X�V
	chr_func::CreateTransMatrix(t, t->m_ModelSize, &t->m_Renderer.m_TransMatrix);

    //���f���X�V
    t->m_Renderer.Update(1);
}


// �X�e�[�g�I��
void TennisState_PlayerControll_Counter::Exit(TennisPlayer* t)
{
	delete m_pCounter;
}
