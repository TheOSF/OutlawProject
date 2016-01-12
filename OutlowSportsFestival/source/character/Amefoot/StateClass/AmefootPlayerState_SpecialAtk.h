#pragma once

#include "../AmefootPlayer.h"
#include "../../../Damage/Damage.h"

//--------------------------------------------------//
//  アメフト必殺技ステート
//--------------------------------------------------//

class AmefootPlayerState_SpecialAtk :public  AmefootState
{
public:
    AmefootPlayerState_SpecialAtk(AmefootPlayer* pCharacter);
    ~AmefootPlayerState_SpecialAtk();

    void Enter(AmefootPlayer* pCharacter)override;

    void Execute(AmefootPlayer* pCharacter)override;

    void Exit(AmefootPlayer* pCharacter)override;

private:
    typedef void(AmefootPlayerState_SpecialAtk::*StateFunc)();
    
    static const float         m_DamageValue;

    AmefootPlayer* const       m_pChr;
    StateFunc                  m_pStateFunc;
    int                        m_Timer;
    DamageControllVanish*      m_pDamage;
    DamageControll_Transform*  m_pDamageTransform;
    

    void SetState(StateFunc state);

    Vector3 GetViewVec();

    bool isHitWall(float Dist);

    void UpdateDamage();

    void UpdateDamageTransform();

    void Explode();

    void NoDamageUpdate();


    void State_Pose();
    void State_Dush();
    void State_DushEnd();
    void State_HangDush();
    void State_WallHit();
    void State_Finish();
};