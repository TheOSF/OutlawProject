#pragma once


#include "../Render/LightObject.h"

//-------------------------------------------------------------//
// 　試合中のライト管理クラス
//-------------------------------------------------------------//

class MatchLightManager
{
public:
    static MatchLightManager& GetInstance();
    static void Release();

    //管理するライト値を追加する
    void AddManageLightValue(
        Vector3* pLightValue
        );

    //色が変わって再び元に戻る
    void LightChangeAndBack(
        UINT      dark_frame,
        UINT      hold_frame,
        UINT      back_frame,
        CrVector3 MulColor
        );

    //ライトの値を更新する
    void UpdateLightColor(
        const Vector3* PreReightValuePtr,
        CrVector3      NewValue
        );

    //毎フレームの更新
    void Update();

private:

    MatchLightManager();
    MatchLightManager(const MatchLightManager&);
    ~MatchLightManager();

    void State_Usual();
    void State_Change();
    void State_Hold();
    void State_Back();



    static MatchLightManager* m_pInstance;
    static const int MaxManageNum = 10;


    void(MatchLightManager::*m_pStateFunc)();

    Vector3  m_pInitValues[MaxManageNum];
    Vector3* m_pManageLightValues[MaxManageNum];
    
    UINT       m_Dark_frame;
    UINT       m_Hold_frame;
    UINT       m_Back_frame;
    Vector3    m_MulColor;

    UINT       m_FrameCount;
};

#define DefMatchLightManager (MatchLightManager::GetInstance())