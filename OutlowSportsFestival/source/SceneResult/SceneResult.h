//*****************************************************************************************************************************
//
//		リザルトシーン
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

    //プレイヤーリザルト情報
    struct PlayerInfo
    {
        int                  Ranking;
        PlayerNum::Value     Num;
        PlayerType::Value    PlayerType;
        CharacterType::Value ChrType;
    };

    //開始に必要なパラメタ
    struct ResultStartParam
    {
        std::list<PlayerInfo> PlayerList;
    };

    SceneResult(
        iex2DObj*                   pScreenTexture,  //スクリーンテクスチャ
        const ResultStartParam&     param
        );

    ~SceneResult();

    void Update();
    void Render();

private:
    enum class NextSceneType
    {
        PreSelect, //未決定

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