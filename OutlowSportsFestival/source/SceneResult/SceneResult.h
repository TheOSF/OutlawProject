//*****************************************************************************************************************************
//
//		���U���g�V�[��
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

    enum class NextSceneType
    {
        PreSelect, //������

        CharacterSelect,
        GameOption,
        End
    };

    SceneResult(
        iex2DObj*                   pScreenTexture,  //�X�N���[���e�N�X�`��
        const ResultStartParam&     param
        );

    ~SceneResult();

    void SetNextScene(NextSceneType Next);

    void Update();
    void Render();

private:
  


    //�V�[���ڍs�N���X
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