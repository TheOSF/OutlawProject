#include "BaseballPlayerState_ComMove.h"

#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_SPAttack_B.h"
#include "../BaseballState_SPAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"
#include "../BaseballState_PoseMotion.h"

#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterManager.h"
#include "BaseballComputerUtillityClass.h"

//�X�e�[�g�J�n
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
{
    {
        //�ړ��p�����[�^����
        CharacterUsualMove::Params p;

        p.Acceleration = 0.15f;
        p.MaxSpeed = 0.28f;
        p.TurnSpeed = 0.3f;
        p.DownSpeed = 0.08f;
        p.RunEndFrame = 35;

        //�ړ��N���X�̍쐬
        m_pMoveClass = new CharacterUsualMove(
            b,
            p,
            new BaseballMoveEvent(b),
            new BaseballHitEvent(b)
            );
    }

    //�ړ��`�h�N���X�̐���
    {
        CharacterComputerMove::InParam param;

        if (b->getBatterFlg())
        {
            param.m_BestLenList.push_back({ 5.0f, 0.7f });
            param.m_BestLenList.push_back({ 20.0f, 0.3f });
        }
        else
        {
            param.m_BestLenList.push_back({ 50.0f, 0.9f });
            param.m_BestLenList.push_back({ 5.0f, 0.1f });
        }

        m_pMoveControllClass = new CharacterComputerMove(
            b,
            param
            );
    }

    //�s���`�h�N���X�̐���
    {
        m_pDoActionClass = new CharacterComputerDoAction(
            b,
            m_pMoveControllClass,
            new BaseballComputerUtillityClass::AttackEvent(b)
            );
    }

    //�����`�h�N���X�̐���
    {
        m_pReactionClass = new CharacterComputerReaction(
            b,
            BaseballComputerUtillityClass::ReactionEvent::GetInParam(b->getBatterFlg()),
            new BaseballComputerUtillityClass::ReactionEvent(b)
            );
    }
}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{

    if (BaseballState_PlayerControll_Move::SwitchGameState(b) == false)
    {
        //�`�h�X�V
        m_pMoveControllClass->Update();

        m_pDoActionClass->Update();

        m_pReactionClass->Update();

        //�X�e�B�b�N�l���Z�b�g
        m_pMoveClass->SetStickValue(m_pMoveControllClass->GetMoveVec());

    }
    else
    {
        //�X�e�B�b�N�̒l�Z�b�g�i�ړ����Ȃ��j
        m_pMoveClass->SetStickValue(Vector2(0, 0));
    }

    //�X�V
    m_pMoveClass->Update();

    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);
}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
	delete m_pDoActionClass;
	delete m_pReactionClass;
}