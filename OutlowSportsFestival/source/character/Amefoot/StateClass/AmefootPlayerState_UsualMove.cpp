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
#include "AmefootComputerState_UsualMove.h"
#include "AmefootCommonState_PoseMotion.h"

//-----------------------------------------------------------------------------------------
// AmefootPlayerState_UsualMove
// [ �A���t�g ] �ʏ�ړ�����X�e�[�g�N���X
//-----------------------------------------------------------------------------------------
AmefootPlayerState_UsualMove::AmefootPlayerState_UsualMove() :
     m_pCharacterUsualMove(nullptr)
{};
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Enter(AmefootPlayer* pCharacter)
{
     CharacterUsualMove::Params params;
     params.Acceleration = 0.085f;
     params.DownSpeed = 0.20f;
     params.MaxSpeed = 0.18f;
     params.TurnSpeed = 0.285f;

     m_pCharacterUsualMove = new CharacterUsualMove(
          pCharacter ,
          params ,
          new AmefootUsualMove_MoveEvent(pCharacter) ,
          new AmefootUsualHitEvent(pCharacter)
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Execute(AmefootPlayer* pCharacter)
{
     this->ActionStateSwitch(pCharacter);

     Vector2 stickScreen = controller::GetStickValue(controller::stick::left , pCharacter->m_PlayerInfo.number);
     Vector3 stickWorld = DefCamera.GetRight() *stickScreen.x + DefCamera.GetForward()* stickScreen.y;
     stickScreen.x = stickWorld.x;
     stickScreen.y = stickWorld.z;

     // �X�e�B�b�N�̒l���Z�b�g
     m_pCharacterUsualMove->SetStickValue(stickScreen);

     // �ړ��N���X�X�V
     m_pCharacterUsualMove->Update();
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterUsualMove;
}
//-----------------------------------------------------------------------------------------
AmefootState* AmefootPlayerState_UsualMove::GetPlayerControllMove(AmefootPlayer* player)
{
    switch ( player->m_PlayerInfo.player_type )
    {
    case PlayerType::_Player:
        return new AmefootPlayerState_UsualMove();

    case PlayerType::_Computer:
        return new AmefootComputerState_UsualMove();

    default:
        break;
    }

    assert("�ʏ�X�e�[�g���쐬�ł��Ȃ��L�����N�^�^�C�v�ł� AmefootPlayerState_UsualMove::GetPlayerControllMove" && 0);
    return nullptr;
}
//-----------------------------------------------------------------------------------------
bool AmefootPlayerState_UsualMove::SwitchGameState(AmefootPlayer* player)
{
    switch (player->GetStateType())
    {
    case CharacterBase::State::Usual:

        return false;

    case CharacterBase::State::Freeze:

        return true;

    case CharacterBase::State::LosePose:
        player->SetState(new AmefootCommonState_PoseMotion(AmefootPlayer::Motion_Lose_Pose, 0.2f, 1000));
        return true;

    case CharacterBase::State::WinPose:
        player->SetState(new AmefootCommonState_PoseMotion(AmefootPlayer::Motion_Win_Pose, 0.2f, 1000));

        return true;
    default:
        break;
    }

    return false;


}

//-----------------------------------------------------------------------------------------
void AmefootPlayerState_UsualMove::ActionStateSwitch(AmefootPlayer* pCharacter)
{
     if ( controller::GetTRG(controller::button::batu, pCharacter->m_PlayerInfo.number))
     {// [�~] �� [���]
          pCharacter->SetState(new AmefootPlayerState_Evasion());
     }

     if (chr_func::isCanSpecialAttack(pCharacter) &&
         controller::GetTRG(controller::button::maru, pCharacter->m_PlayerInfo.number))
     {
         // [��] �� [�K�E�Z]
         pCharacter->SetState(new AmefootPlayerState_SpecialAtk(pCharacter));
     }
     
     if (controller::GetTRG(controller::button::sankaku, pCharacter->m_PlayerInfo.number) && 
         pCharacter->isCanThrowBall()
         )
     {// [��] �� [�������U��]
         pCharacter->SetState(new AmefootPlayerState_ThrowBall());
             
     }
     
     if ( controller::GetTRG(controller::button::shikaku, pCharacter->m_PlayerInfo.number) )
     {// [��] �� [�ߋ����U��]
         pCharacter->SetState(
             new AmefootPlayerState_Tackle(pCharacter)
             );
     }
     
     if ( controller::GetTRG(controller::button::_R1, pCharacter->m_PlayerInfo.number) )
     {// [R1] �� [�J�E���^�[]
          pCharacter->SetState(new AmefootPlayerState_Counter());
     }
     
     if ( controller::GetTRG(controller::button::_L1, pCharacter->m_PlayerInfo.number) )
     {

     }
}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// MoveEvent
//-----------------------------------------------------------------------------------------
AmefootUsualMove_MoveEvent::AmefootUsualMove_MoveEvent(AmefootPlayer* pAmefoot) :
     m_pAmefoot(pAmefoot)
{}
//-----------------------------------------------------------------------------------------
void AmefootUsualMove_MoveEvent::Update(bool isRun, RATIO speed)
{
     m_pAmefoot->m_Renderer.Update(1);

     // ���f���̃��[���h�ϊ��s����X�V
     chr_func::CreateTransMatrix(
          m_pAmefoot ,
          &m_pAmefoot->m_Renderer.m_TransMatrix
          );
}
//-----------------------------------------------------------------------------------------
void AmefootUsualMove_MoveEvent::RunStart()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Run);
}
//-----------------------------------------------------------------------------------------
void AmefootUsualMove_MoveEvent::StandStart()
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Stand);
}
//-----------------------------------------------------------------------------------------
void AmefootUsualMove_MoveEvent::RunEnd()
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Run_End);
}
//-----------------------------------------------------------------------------------------




