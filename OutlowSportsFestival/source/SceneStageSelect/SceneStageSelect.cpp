#include "SceneStageSelect.h"
#include "../SceneOption/SceneOption.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"
#include "../GameSystem/GameController.h"

#include "../Sound/Sound.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"

SceneStageSelect::SceneStageSelect(sceneGamePlay::InitParams param)
	:initparam(param),WhiteAlpha(1.0f),WakuX(362),TitleY(-150),GroundX(1862),KasenX(-600),Time(0)
{
	iexLight::SetFog(800, 1000, 0);
	Back = new iex2DObj("DATA\\Texture\\sports.png");
	Kasen = new iex2DObj("DATA\\Texture\\Kasen.png");
	Ground = new iex2DObj("DATA\\Texture\\Ground.png");
	Waku1 = new iex2DObj("DATA\\Texture\\Waku1.png");
	Waku2 = new iex2DObj("DATA\\Texture\\Waku2.png");
	StageSelect = new iex2DObj("DATA\\Texture\\stageselect.png");
	White = new iex2DObj("DATA\\Texture\\white.png");
	Decide = new iex2DObj("DATA\\Texture\\Decidion.png");
	stagetexture = Groundtex;
	step = TitleMove;

}
SceneStageSelect::~SceneStageSelect()
{
	delete Back;
	delete Kasen;
	delete Ground;
	delete Waku1;
	delete Waku2;
	delete StageSelect;
	delete White;
	delete Decide;
}

void SceneStageSelect::Update()
{
	switch (step)
	{
	case TitleMove:
		TitleY += 20;
		if (TitleY >= 0)
		{
			TitleY = 0;
			step = TexMove;
		}
		break;
	case TexMove:
		GroundX -= 50;
		KasenX += 50;
		if (GroundX <= 362)
		{
			GroundX = 362;
			KasenX = 900;
			step = Select;
		}
		break;
	case Select:
		Time++;
		WakuUpdate();
		if (controller::GetTRG(controller::button::maru, 0))
		{
			step = Decidion;
			//MainFrame->ChangeScene(new sceneGamePlay(initparam));
		}
		if (controller::GetTRG(controller::button::batu, 0))
		{
			MainFrame->ChangeScene(new SceneOption());
		}
		break;
	case Decidion:
		WhiteAlpha -= .01f;
		if (WhiteAlpha<=0.0f)
		{
			step = leave;
			WhiteAlpha = 0;
		}
		
		
		break;
	case leave:
		KasenX -= 70;
		GroundX += 70;
		TitleY -= 20;
		if (KasenX <= -600)
		{
			KasenX = -600;
			GroundX = 1800;
			MainFrame->ChangeScene(new sceneGamePlay(initparam));
		}
		break;
	}
	
}
void SceneStageSelect::Render()
{
	Back->Render(0, 0, 1944, 1137, 0, 0, 500, 500);//ƒoƒbƒN
	StageSelect->Render(600, TitleY, 512, 128, 0, 0, 512, 128);//
	
	Ground->Render(GroundX, 200, 512, 512, 0, 0, 512, 512);
	Kasen->Render(KasenX, 200, 512, 512, 0, 0, 512, 512);
	if (step == Select)
	{
		WakuRender();
	}
	if (step == Decidion || step == leave)
	{
		SelectRender();
	}
}
void SceneStageSelect::WakuRender()
{
	if (Time % 10 < 5)
	{
		Waku1->Render(WakuX, 200, 430, 430, 0, 0, 512, 512);
	}
	else
	{
		Waku2->Render(WakuX, 200, 430, 430, 0, 0, 512, 512);
	}

}
void SceneStageSelect::SelectRender()
{
	switch (stagetexture)
	{
		GameInitializer_UsualMatch::StageType Stage;
	case Groundtex:
		Stage = GameInitializer_UsualMatch::StageType::School;
		initparam.pInitializer = new GameInitializer_UsualMatch(Stage, initparam);
		Decide->Render(GroundX, 200, 430, 430, 0, 0, 512, 512);
		White->RenderPlus(Vector2((float)GroundX+215, 200+215), Vector2(430, 430), 0, Vector2(0, 0), Vector2(100, 100), RS_COPY,
			D3DCOLOR_COLORVALUE(1, 1, 1, WhiteAlpha), 0);
		break;
	case Kasentex:
		Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
		initparam.pInitializer = new GameInitializer_UsualMatch(Stage, initparam);
		Decide->Render(KasenX, 200, 430, 430, 0, 0, 512, 512);
		White->RenderPlus(Vector2((float)KasenX+215, 200+215), Vector2(430, 430), 0, Vector2(0, 0), Vector2(100, 100), RS_COPY,
			D3DCOLOR_COLORVALUE(1, 1, 1, WhiteAlpha), 0);
		break;

	}

}
void SceneStageSelect::WakuUpdate()
{
	switch (stagetexture)
	{
	case Groundtex:
		if (controller::GetTRG(controller::button::right, 0))
		{
			WakuX = 900;
			stagetexture = Kasentex;
		}
		else if (controller::GetTRG(controller::button::left, 0))
		{
			WakuX = 900;
			stagetexture = Kasentex;
		}
		break;
	case Kasentex:
		if (controller::GetTRG(controller::button::right, 0))
		{
			WakuX = 362;
			stagetexture = Groundtex;
		}
		else if (controller::GetTRG(controller::button::left, 0))
		{
			WakuX = 362;
			stagetexture = Groundtex;
		}
		break;
	}

}

