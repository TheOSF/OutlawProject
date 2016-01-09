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
using namespace std;

class SceneStageSelect : public Scene
{
private:
    enum Stagetex{
        Kasentex,
        Groundtex
    };
    enum Step {
        TitleMove,
        TexMove,
        Select,
        Decidion,
        leave
    };

	int Time;
	int WakuX;
	int GroundX;
	int KasenX;
	int TitleY;
	float WhiteAlpha;

	DWORD GroundColor;
	DWORD KasenColor;
public:
	SceneStageSelect(sceneGamePlay::InitParams param);
	~SceneStageSelect();

	Stagetex stagetexture;
	Step step;
	sceneGamePlay::InitParams initparam;

	iex2DObj* Back;//タイトル画像
	iex2DObj* Waku1;
	iex2DObj* Waku2;
	iex2DObj* Kasen;
	iex2DObj* Ground;
	iex2DObj* White;
	iex2DObj* StageSelect;
	iex2DObj* Decide;

	

	void WakuUpdate();
	void SelectUpdate();
	void Update();
	void WakuRender();
	void SelectRender();
	void Render();

};