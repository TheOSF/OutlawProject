#pragma once


#include "../AmefootPlayer.h"


//-----------------------------------------------------------------------------------------
// AmefootPlayerState_ThrowBall
// [ アメフト ] 遠距離攻撃(ボール投げ)をするステートクラス
//-----------------------------------------------------------------------------------------
class AmefootPlayerState_ThrowBall : public AmefootState
{
public:
    class ControllClass
    {
    public:
        virtual ~ControllClass() {}
        virtual Vector3 GetVec() = 0;
        virtual bool DoOtherAction() = 0;
        virtual bool DoShotAfterAction() = 0;
    };


    AmefootPlayerState_ThrowBall(ControllClass* pControllClass);


    ~AmefootPlayerState_ThrowBall();


    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    int m_Timer;
    ControllClass* m_pControllClass;
};
//-----------------------------------------------------------------------------------------

