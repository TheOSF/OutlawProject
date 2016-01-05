#include "MatchLightManager.h"

MatchLightManager* MatchLightManager::m_pInstance = nullptr;



MatchLightManager& MatchLightManager::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new MatchLightManager();
    }

    return *m_pInstance;
}

void MatchLightManager::Release()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}



//�Ǘ����郉�C�g�l��ǉ�����
void MatchLightManager::AddManageLightValue(
    Vector3* pLightValue
    )
{
    for (int i = 0; i < MaxManageNum; ++i)
    {
        if (m_pManageLightValues[i] != nullptr)
        {
            continue;
        }

        m_pInitValues[i] = *pLightValue;
        m_pManageLightValues[i] = pLightValue;

        return;
    }

    MyAssert(false, "�Ǘ��ł��郉�C�g�̌����ő�B���܂���");
}

//�F���ς���čĂь��ɖ߂�
void MatchLightManager::LightChangeAndBack(
    UINT   dark_frame,
    UINT   hold_frame,
    UINT   back_frame,
    CrVector3 ToColor
    )
{
    m_pStateFunc = &MatchLightManager::State_Change;

    LightInit();

    m_FrameCount = 0;

    m_Dark_frame = dark_frame;
    m_Hold_frame = hold_frame;
    m_Back_frame = back_frame;
    m_MulColor = ToColor;
}


//���C�g�̒l���X�V����
void MatchLightManager::UpdateLightColor(
    const Vector3* PreReightValuePtr,
    CrVector3      NewValue
    )
{
    for (int i = 0; i < MaxManageNum; ++i)
    {
        if (m_pManageLightValues[i] == PreReightValuePtr)
        {
            m_pInitValues[i] = NewValue;
            return;
        }
    }

    MyAssert(false, "���C�g�����Ɏ��s");
}

//���t���[���̍X�V
void MatchLightManager::Update()
{
    (this->*m_pStateFunc)();
}


MatchLightManager::MatchLightManager() :
m_FrameCount(0)
{
    m_pStateFunc = &MatchLightManager::State_Usual;

    for (int i = 0; i < MaxManageNum; ++i)
    {
        m_pManageLightValues[i] = nullptr;
    }
}

MatchLightManager::~MatchLightManager()
{

}



void MatchLightManager::State_Usual()
{
    //�������Ȃ�
    
}

void MatchLightManager::State_Change()
{
    //�F�����񂾂�ς��Ă���

    const RATIO t = (float)m_FrameCount / (float)m_Dark_frame;

    for (int i = 0; i < MaxManageNum; ++i)
    {
        if (m_pManageLightValues[i] == nullptr)
        {
            break;
        }



        *m_pManageLightValues[i] = 
            m_pInitValues[i] * (1 - t) + 
            Vector3(
                m_pInitValues[i].x*m_MulColor.x,
                m_pInitValues[i].y*m_MulColor.y,
                m_pInitValues[i].z*m_MulColor.z
            )*t;
    }

    if (++m_FrameCount > m_Dark_frame)
    {
        m_FrameCount = 0;
        m_pStateFunc = &MatchLightManager::State_Hold;
    }
}

void MatchLightManager::State_Hold()
{
    //�z�[���h
    if (++m_FrameCount > m_Hold_frame)
    {
        m_FrameCount = 0;
        m_pStateFunc = &MatchLightManager::State_Back;
    }
}

void MatchLightManager::State_Back()
{
    //���ɖ߂�
    const RATIO t = (float)m_FrameCount / (float)m_Back_frame;

    for (int i = 0; i < MaxManageNum; ++i)
    {
        if (m_pManageLightValues[i] == nullptr)
        {
            break;
        }

        *m_pManageLightValues[i] =
            m_pInitValues[i] * t + Vector3(
            m_pInitValues[i].x*m_MulColor.x,
            m_pInitValues[i].y*m_MulColor.y,
            m_pInitValues[i].z*m_MulColor.z
            )*(1 - t);
    }

    if (++m_FrameCount > m_Back_frame)
    {
        m_FrameCount = 0;
        m_pStateFunc = &MatchLightManager::State_Usual;
    }
}

void MatchLightManager::LightInit()
{
    for (int i = 0; i < MaxManageNum; ++i)
    {
        if (m_pManageLightValues[i] == nullptr)
        {
            break;
        }

        *m_pManageLightValues[i] = m_pInitValues[i];
    }
}