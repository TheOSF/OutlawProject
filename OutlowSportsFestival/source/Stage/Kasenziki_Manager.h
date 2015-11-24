#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/LightObject.h"
#include "SkyObject.h"
//------------------------------------------------------//
// 河川敷の管理を行うオブジェクト
//------------------------------------------------------//


/*

* 主な仕事

オブジェクトの設置
車オブジェクト生成
ライティング管理
物理オブジェクトリセット
バス突撃

*/

class Kasennziki_Manager:public GameObjectBase
{
public:
    Kasennziki_Manager(UINT night_round);
    ~Kasennziki_Manager();

    void CreateStage();

private:
    enum
    {
        _NumSpotLight = 2
    };
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