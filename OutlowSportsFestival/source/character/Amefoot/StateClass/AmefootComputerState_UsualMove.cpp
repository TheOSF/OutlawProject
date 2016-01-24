#include "AmefootComputerState_UsualMove.h"
#include "AmefootPlayerState_UsualMove.h"
#include "../AmefootPlayerState.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../../GameSystem/GameController.h"
#include "../../../Camera/Camera.h"
#include "../../CharacterFunction.h"
#include "../../CharacterHitEventFunc.h"
#include "../AmefootUsualHitEvent.h"
#include "../AmefootPlayerState.h"
#include "../UtilityClass/AmefootComputerUtillityClass.h"

//-----------------------------------------------------------------------------------------
// AmefootComputerState_UsualMove
// [ �A���t�g ] �ʏ�ړ�����X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
AmefootComputerState_UsualMove::AmefootComputerState_UsualMove()
{};
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Enter(AmefootPlayer* pCharacter)
{
    {
        CharacterUsualMove::Params params;
        params.Acceleration = 0.085f;
        params.DownSpeed = 0.20f;
        params.MaxSpeed = 0.18f;
        params.TurnSpeed = 0.285f;

        m_pMoveClass = new CharacterUsualMove(
            pCharacter,
            params,
            new AmefootUsualMove_MoveEvent(pCharacter),
            new AmefootUsualHitEvent(pCharacter)
            );
    }

    //�ړ��`�h�N���X�̐���
    {
        CharacterComputerMove::InParam param;

        param.m_BestLenList.push_back({ 5.0f, 0.7f });
        param.m_BestLenList.push_back({ 20.0f, 0.3f });

        m_pCharacterComputerMove = new CharacterComputerMove(
            pCharacter,
            param
            );
    }

    //�s���`�h�N���X�̐���
    {
        m_pCharacterComputerDoAction = new CharacterComputerDoAction(
            pCharacter,
            m_pCharacterComputerMove,
            new AmefootComputerUtillityClass::AttackEvent(pCharacter)
            );
    }

    //�����`�h�N���X�̐���
    {
        m_pCharacterComputerReaction = new CharacterComputerReaction(
            pCharacter,
            AmefootComputerUtillityClass::ReactionEvent::GetInParam(),
            new AmefootComputerUtillityClass::ReactionEvent(pCharacter)
            );
    }
}
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Execute(AmefootPlayer* pCharacter)
{
    if (AmefootPlayerState_UsualMove::SwitchGameState(pCharacter) == false)
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
    chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);

}
//-----------------------------------------------------------------------------------------
void AmefootComputerState_UsualMove::Exit(AmefootPlayer* pCharacter)
{
    delete m_pMoveClass;
    delete m_pCharacterComputerMove;
    delete m_pCharacterComputerDoAction;
    delete m_pCharacterComputerReaction;
}

