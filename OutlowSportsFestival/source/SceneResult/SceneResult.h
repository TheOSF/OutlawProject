//*****************************************************************************************************************************
//
//		リザルトシーン
//
//*****************************************************************************************************************************
#pragma once

#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "../GameSystem/GameSystem.h"
#include "../UI/SelectMenuUI.h"
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

    enum class NextSceneType
    {
        PreSelect, //未決定

        CharacterSelect,
        GameOption,
        End
    };

    SceneResult(
        iex2DObj*                   pScreenTexture,  //スクリーンテクスチャ
        const ResultStartParam&     param
        );

    ~SceneResult();

    void SetNextScene(NextSceneType Next);

    void Update();
    void Render();

private:
  


    //シーン移行クラス
    class SceneChangeDecide :public SelectMenuUI::IDecideEvent
    {
    public:
        SceneChangeDecide(SceneResult* Result, NextSceneType Next);

        void Decide()override;

    private:
        SceneResult* const  m_pResult;
        const NextSceneType m_Next;
    };


    iex2DObj*           m_pScreenTexture;
    iex3DObj*           m_pBreakScreenModel;
    ResultStartParam    m_StartParam;
    NextSceneType       m_NextSceneType;
    int                 m_Timer;

    void(SceneResult::*m_pStateFunc)();

    void State_Initialize();
    void State_ScreenVanish();
    void State_Back();
    
    void SetLights();
    void SetCharacter(CrVector3 pos, PlayerInfo& info);
};