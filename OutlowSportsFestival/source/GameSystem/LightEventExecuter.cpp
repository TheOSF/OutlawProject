#include "LightEventExecuter.h"

LightEventExecuter::LightEventExecuter(
    LPVECTOR3 pDirLightColor,
    LPVECTOR3 pAmbLightColor
    ):
    m_pDirLightColor(pDirLightColor),
    m_pAmbLightColor(pAmbLightColor),
    m_ChrLiveCount(0)
{
    m_InitDir = *m_pDirLightColor;
    m_InitAmb = *m_pAmbLightColor;
}

LightEventExecuter::~LightEventExecuter()
{

}

bool LightEventExecuter::Update()
{
    if (EventCheck())
    {
        LightOff();
    }

    LightUpdate();

    return true;
}

bool LightEventExecuter::Msg(MsgType mt)
{
    return false;
}

bool LightEventExecuter::EventCheck()
{
    bool ret = false;

    if (m_ChrLiveCount == 0)
    {
        m_ChrLiveCount = DefCharacterMgr.GetCharacterLiveCount();
    }

    //キャラクタ死亡チェック
    if (m_ChrLiveCount != DefCharacterMgr.GetCharacterLiveCount())
    {
        ret = true;
    }

    m_ChrLiveCount = DefCharacterMgr.GetCharacterLiveCount();

    return ret;
}

void LightEventExecuter::LightOff()
{
    *m_pDirLightColor = Vector3Zero;
}

void LightEventExecuter::LightUpdate()
{
    *m_pDirLightColor += (m_InitDir - *m_pDirLightColor)*0.025f;
}