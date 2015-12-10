#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/LightObject.h"
#include "SkyObject.h"

//------------------------------------------------------//
// �Z�ɃO���E���h�̊Ǘ����s���I�u�W�F�N�g
//------------------------------------------------------//


/*

* ��Ȏd��

*/

class Ground_Manager :public GameObjectBase
{
public:
    Ground_Manager(UINT night_round);
    ~Ground_Manager();

    void CreateStage();

private:
    static const char* StageObjFileName;

    const UINT     m_SunsetRound;
    UINT           m_RoundCount;

    DirLight       m_DirLight;
    AmbientLight   m_AmbientLight;
    SkyObject*     m_pSkyObject;
    UINT           m_DeleyFrame;


    void RoundReset();

    void SetSunsetObject();
    void SetNoonObject();

    void SetSunsetLight();
    void SetNoonLight();

    bool Update();
    bool Msg(MsgType msg);
};