#include "AmefootPlayerState_Counter.h"
#include "../../CharacterDefaultCounterClass.h"
#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../AmefootUsualHitEvent.h"
#include "AmefootPlayerState_UsualMove.h"


//-----------------------------------------------------------------------------------------
// CounterEvent
//-----------------------------------------------------------------------------------------
typedef class AmefootPlayerState_Counter::CounterEvent : 
     public CharacterDefaultCounter::Event
{
public:
     CounterEvent(AmefootPlayer* pAmefoot);

     void Pose()override;

     void Move(BallBase* pCounterBall)override;

     void Catch(BallBase* pCounterBall)override;

     void Shot(BallBase* pCounterBall)override;

     void ShotFaild()override;

     void End()override;

     Vector3 ClacLocalOffset(bool Right)override;

private:
     AmefootPlayer* m_pAmefoot;
     bool           m_isMotionRight;
}AmefootCounter_Event;



//-----------------------------------------------------------------------------------------
// AmefootPlayerState_Counter
// [ アメフト ] カウンターをするステートクラス
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Counter::AmefootPlayerState_Counter() :
     m_pCharacterDefaultCounter(nullptr)
{};
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::Enter(AmefootPlayer* pCharacter)
{
     CharacterDefaultCounter::Param params;

     params.AfterShotFrame = 11;
     params.CanCounterFrame = 25;
     params.CatchAriaSize = 5.5f;
     params.ControllRadian = D3DXToRadian(65);
     params.FailedFrame = 48;
     params.PoseFrame = 20;
     params.ShotFrame = 5;
     params.CatchFrame = 18;
     params.CatchBoneNumber = 32;
     params.BallSpeed = 0.78f;


     m_pCharacterDefaultCounter = new CharacterDefaultCounter(
          pCharacter,
          params,
          new AmefootCounter_Event(pCharacter),
          new AmefootUsualHitEvent(pCharacter)
          );
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::Execute(AmefootPlayer* pCharacter)
{
     // スティックの値セット
     m_pCharacterDefaultCounter->SetStickValue(
          controller::GetStickValue(controller::stick::left, pCharacter->m_PlayerInfo.number));

     // 更新
     m_pCharacterDefaultCounter->Update();

     // モデルのワールド変換行列を更新
     chr_func::CreateTransMatrix(pCharacter, &pCharacter->m_Renderer.m_TransMatrix);

     // モデル更新
     pCharacter->m_Renderer.Update(1);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::Exit(AmefootPlayer* pCharacter)
{
     delete m_pCharacterDefaultCounter;
}
//-----------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------
// CounterEvent
//-----------------------------------------------------------------------------------------
AmefootPlayerState_Counter::CounterEvent::CounterEvent(AmefootPlayer* pAmefoot) :
m_pAmefoot(pAmefoot),
m_isMotionRight(true)
{};
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::Pose()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Counter_Pose);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::Move(BallBase* pCounterBall)
{
    if (m_isMotionRight)
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Counter_Catch_Right);
    }
    else
    {
        m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Counter_Catch_Left);
    }
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::Catch(BallBase* pCounterBall)
{
    m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Counter_Throw_Ball);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::Shot(BallBase* pCounterBall)
{

}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::ShotFaild()
{
     m_pAmefoot->m_Renderer.SetMotion(AmefootPlayer::Motion_Counter_Failed);
}
//-----------------------------------------------------------------------------------------
void AmefootPlayerState_Counter::CounterEvent::End()
{
    m_pAmefoot->SetState(AmefootPlayerState_UsualMove::GetPlayerControllMove(m_pAmefoot));
}
//-----------------------------------------------------------------------------------------
Vector3 AmefootPlayerState_Counter::CounterEvent::ClacLocalOffset(bool Right)
{
    Vector3 ret(0, 0, 0);
    m_isMotionRight = Right;

    //ボールの位置によってモーション分岐
    if (Right)
    {
        ret = Vector3(1.5f, 0, 0);
    }
    else
    {
        ret = Vector3(-1.0f, 0, 0);
    }
    return ret;
}
