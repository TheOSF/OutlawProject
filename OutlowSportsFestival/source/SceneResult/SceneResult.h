//*****************************************************************************************************************************
//
//		���U���g�V�[��
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "../GameSystem/GameSystem.h"
#include <list>

class SceneResult : public Scene
{
public:

    //�v���C���[���U���g���
    struct PlayerInfo
    {
        int                  Ranking;
        PlayerNum::Value     Num;
        PlayerType::Value    PlayerType;
        CharacterType::Value ChrType;
    };

    //�J�n�ɕK�v�ȃp�����^
    struct ResultStartParam
    {
        std::list<PlayerInfo> PlayerList;
    };

    SceneResult(
        iex2DObj*                   pScreenTexture,  //�X�N���[���e�N�X�`��
        const ResultStartParam&     param
        );

    ~SceneResult();

    void Update();
    void Render();

private:
    enum class NextSceneType
    {
        PreSelect, //������

        CharacterSelect,
        GameOption,
        End
    };

    iex2DObj*           m_pScreenTexture;
    iex3DObj*           m_pBreakScreenModel;
    ResultStartParam    m_StartParam;
    NextSceneType       m_NextSceneType;

    void(SceneResult::*m_pStateFunc)();

    void State_Initialize();
    void State_ScreenVanishAnd();
    void State_ToWinCharacterZoom();

};