#pragma once
//*****************************************************************************************************************************
//
//		ステージセレクトシーン
//
//*****************************************************************************************************************************
#pragma once
#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "../GameScene/SceneGamePlay.h"

//
//using namespace std;
//
//class SceneStageSelect : public Scene
//{
//private:
//    enum Stagetex{
//        Kasentex,
//        Groundtex
//    };
//    enum Step {
//        TitleMove,
//        TexMove,
//        Select,
//        Decidion,
//        leave
//    };
//
//	int Time;
//	int WakuX;
//	int GroundX;
//	int KasenX;
//	int TitleY;
//	float WhiteAlpha;
//
//	DWORD GroundColor;
//	DWORD KasenColor;
//public:
//	SceneStageSelect(sceneGamePlay::InitParams param);
//	~SceneStageSelect();
//
//	Stagetex stagetexture;
//	Step step;
//	sceneGamePlay::InitParams initparam;
//
//	iex2DObj* Back;//タイトル画像
//	iex2DObj* Waku1;
//	iex2DObj* Waku2;
//	iex2DObj* Kasen;
//	iex2DObj* Ground;
//	iex2DObj* White;
//	iex2DObj* StageSelect;
//	iex2DObj* Decide;
//
//	
//
//	void WakuUpdate();
//	void SelectUpdate();
//	void Update();
//	void WakuRender();
//	void SelectRender();
//	void Render();
//
//};
//
//

class SceneStageSelect : public Scene
{
public:
    SceneStageSelect(sceneGamePlay::InitParams& param);
    ~SceneStageSelect();


    bool Initialize()override;

    void Update()override;
    void Render()override;

private:

    sceneGamePlay::InitParams       m_Param;

};


//class SceneStageSelect : public Scene
//{
//public:
//    SceneStageSelect(sceneGamePlay::InitParams& param);
//    ~SceneStageSelect();
//
//
//    bool Initialize()override;
//
//    void Update()override;
//    void Render()override;
//
//private:
//
//    struct TextureParam
//    {
//        ~TextureParam();
//
//        iex2DObj*   pTexture;
//        RectI       TexRect;
//        RectI       DrawRect;
//    };
//
//    struct StageParam
//    {
//        TextureParam StageTexParam;
//        TextureParam NameTexParam;
//    };
//
//    typedef std::list<StageParam> StageParamList;
//    typedef void(SceneStageSelect::*StateFunc)();
//
//    static const float StageListSpaceX;
//
//    sceneGamePlay::InitParams   m_Param;
//    StageParamList              m_StageParamList;
//    StageParamList::iterator    m_NowSelect;
//    StateFunc                   m_pStateFunc;
//    int                         m_Timer;
//    float                       m_SelectX;
//    
//    void LoadStage();
//
//    void SetState(StateFunc state);
//    
//    void State_In();
//    void State_Selecting();
//    void State_SceneNext();
//    void State_SceneBack();
//};
//
