#pragma once


#include "../Render/LightObject.h"

//-------------------------------------------------------------//
// �@�������̃��C�g�Ǘ��N���X
//-------------------------------------------------------------//

class MatchLightManager
{
public:
    static MatchLightManager& GetInstance();
    static void Release();

    //�Ǘ����郉�C�g�l��ǉ�����
    void AddManageLightValue(
        Vector3* pLightValue
        );

    //�F���ς���čĂь��ɖ߂�
    void LightChangeAndBack(
        UINT      dark_frame,
        UINT      hold_frame,
        UINT      back_frame,
        CrVector3 MulColor
        );

    //���C�g�̒l���X�V����
    void UpdateLightColor(
        const Vector3* PreReightValuePtr,
        CrVector3      NewValue
        );

    //���t���[���̍X�V
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