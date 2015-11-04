#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/LightObject.h"

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
    
    const UINT     m_NightRound;
    UINT           m_RoundCount;

    DirLight       m_DirLight;
    AmbientLight   m_AmbientLight;
    SpotLight*     m_pSpotLight[_NumSpotLight];

    UINT           m_DeleyFrame;


    void RoundReset();

    void SetSunsetObject();
    void SetNightObject();

    void SetSunsetLight();
    void SetNightLight();

    bool Update();
    bool Msg(MsgType msg);
};