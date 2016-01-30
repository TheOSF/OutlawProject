#include "../Computer/SoccerComputerMove.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterFunction.h"
#include "../../CharacterMoveClass.h"
#include "SoccerComputerDash.h"
#include "../SoccerPlayerState.h"
#include "../SoccerRolling.h"
#include "../SoccerPlayerState_PoseMotion.h"
#include "../../CharacterManager.h"
#include "../../../Camera/Camera.h"
#include "../SoccerHitEvent.h"
#include "../../CharacterManager.h"

#include "../Computer/SoccerComputerUtilityClass.h"
#include "../SoccerAttackState.h"



//�X�e�[�g�J�n
void SoccerState_ComputerControll_Move::Enter(SoccerPlayer* s)
{
    {
        CharacterUsualMove::Params p;

        p.Acceleration = 0.25f;
        p.MaxSpeed = 0.35f;
        p.TurnSpeed = 0.45f;
        p.DownSpeed = 0.08f;
        p.RunEndFrame = 35;


        m_pMoveClass = new CharacterUsualMove(
            s,
            p,
            new SoccerMoveEvent(s),
            new SoccerHitEvent(s)
            );
    }

    //�ړ��`�h�N���X�̐���
    {
        CharacterComputerMove::InParam param;

        param.m_BestLenList.push_back({ 5.0f, 0.3f });
        param.m_BestLenList.push_back({ 50.0f, 0.7f });

        m_pCharacterComputerMove = new CharacterComputerMove(
            s,
            param
            );
    }

    //�s���`�h�N���X�̐���
    {
        m_pCharacterComputerDoAction = new CharacterComputerDoAction(
            s,
            m_pCharacterComputerMove,
            new SocceComputerrUtillityClass::AttackEvent(s)
            );
    }

    //�����`�h�N���X�̐���
    {
        m_pCharacterComputerReaction = new CharacterComputerReaction(
            s,
            SocceComputerrUtillityClass::ReactionEvent::GetInParam(),
            new SocceComputerrUtillityClass::ReactionEvent(s)
            );
    }
}


void SoccerState_ComputerControll_Move::Execute(SoccerPlayer* s)
{
    if (SoccerState_PlayerControll_Move::SwitchGameState(s) == false)
    {
        //�`�h�X�V
        m_pCharacterComputerMove->Update();

        m_pCharacterComputerDoAction->Update();

        m_pCharacterComputerReaction->Update();

        //�X�e�B�b�N�l���Z�b�g
        m_pMoveClass->SetStickValue(m_pCharacterComputerMove->GetMoveVec());

    }
    else
    {
        //�X�e�B�b�N�̒l�Z�b�g�i�ړ����Ȃ��j
        m_pMoveClass->SetStickValue(Vector2(0, 0));
    }

    //�X�V
    m_pMoveClass->Update();

    //���f���̃��[���h�ϊ��s����X�V
    chr_func::CreateTransMatrix(s, &s->m_Renderer.m_TransMatrix);


}

void SoccerState_ComputerControll_Move::Exit(SoccerPlayer* s)
{
    delete m_pMoveClass;
    delete m_pCharacterComputerMove;
    delete m_pCharacterComputerDoAction;
    delete m_pCharacterComputerReaction;
}