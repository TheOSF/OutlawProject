#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/LightObject.h"
#include "SkyObject.h"
//------------------------------------------------------//
// �͐�~�̊Ǘ����s���I�u�W�F�N�g
//------------------------------------------------------//


/*

* ��Ȏd��

�I�u�W�F�N�g�̐ݒu
�ԃI�u�W�F�N�g����
���C�e�B���O�Ǘ�
�����I�u�W�F�N�g���Z�b�g
�o�X�ˌ�

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