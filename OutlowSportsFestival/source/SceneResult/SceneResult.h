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
        std::list<PlayerInfo>   PlayerList;
        UINT                    Round;
        UINT                    RoundFrame;
        
        UINT GetPlayerNum()const;
    };

    enum class NextSceneType
    {
        PreSelect, //未決定

        CharacterSelect,
        Title,
        GameOption,
        StaffRoll,
    };

    SceneResult(
        iex2DObj*                   pScreenTexture,  //スクリーンテクスチャ
        const ResultStartParam&     param
        );

    ~SceneResult();

    bool Initialize();

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


    ResultStartParam    m_StartParam;
    NextSceneType       m_NextSceneType;
    int                 m_Timer;
    iex2DObj*           m_pScreenTex;
    iex2DObj*           m_pUITex;
    iex2DObj*           m_pPlayerNumTex;

    void(SceneResult::*m_pStateFunc)();

    void GetWinChrCameraParam(Vector3& pos, Vector3& target);

    int GetWinCharacterPlayerNum();

    Vector3 GetChrPosByNum(int num);

    void State_Initialize();
    void State_ScreenVanish();
    void State_Back();
    
    void CreateStage();
    void CreateCharacterBlock(CrVector3 Pos,int Rank);

    void SetLights();
    void SetCharacter(CrVector3 pos, PlayerInfo& info);
};