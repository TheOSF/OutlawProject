#include "TennisPlayerState_ComMove.h"
#include "../TennisPlayerState_Counter.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../CharacterManager.h"
#include "../Tennis_HitEvent.h"
#include "../TennisPlayerState_Attack.h"
#include "../TennisUtillityClass.h"
#include "TennisComputerUtillityClass.h"
#include "TennisComputerReactionEvent.h"
#include "../TennisPlayerState_UsualMove.h"

//�X�e�[�g�J�n
void TennisState_ComputerControll_Move::Enter(TennisPlayer* t)
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
            t,
            p,
            new TennisUtillityClass::TennisMoveEvent(t),
            new TennisHitEvent(t)
            );
    }

    //�ړ��`�h�N���X�̐���
    {
        CharacterComputerMove::InParam param;

        param.m_BestLenList.push_back({ 40.0f, 0.6f });
        param.m_BestLenList.push_back({ 20.0f, 0.3f });
        param.m_BestLenList.push_back({ 5.0f, 0.1f });

        m_pCharacterComputerMove = new CharacterComputerMove(
            t,
            param
            );
    }

    //�s���`�h�N���X�̐���
    {
        m_pCharacterComputerDoAction = new CharacterComputerDoAction(
            t,
            m_pCharacterComputerMove,
            new TennisComputerUtillityClass::AttackEvent(t)
            );
    }

    //�����`�h�N���X�̐���
    {
        m_pCharacterComputerReaction = new CharacterComputerReaction(
            t,
            TennisComputerReactionEvent::GetCharacterComputerReactionInParam(),
            new TennisComputerReactionEvent(t)
            );
    }
}


void TennisState_ComputerControll_Move::Execute(TennisPlayer* t)
{
    if (TennisState_PlayerControll_Move::SwitchGameState(t) == false)
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
    chr_func::CreateTransMatrix(t, &t->m_Renderer.m_TransMatrix);

}

void TennisState_ComputerControll_Move::Exit(TennisPlayer* t)
{
    delete m_pMoveClass;
    delete m_pCharacterComputerMove;
    delete m_pCharacterComputerDoAction;
    delete m_pCharacterComputerReaction;
}

