#include "SceneStageSelect.h"
#include "../SceneOption/SceneOption.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"
#include "../GameSystem/GameController.h"

#include "../Sound/Sound.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"

static POSITON_SIZE STAGE_POS[2] =
{
	{ 302, 155, 576, 576, 576, 576 },
	{ 509, 157, 576, 576, 200, 200 },
};

SceneStageSelect::SceneStageSelect(sceneGamePlay::InitParams param)
	:initparam(param),WhiteAlpha(1.0f),WakuX(162),TitleY(-350),GroundX(1910),KasenX(-600),Time(0)
{
	iexLight::SetFog(800, 1000, 0);
	Back = new iex2DObj("DATA\\Texture\\sports.png");
	Kasen = new iex2DObj("DATA\\Texture\\Kasen.png");
	Ground = new iex2DObj("DATA\\Texture\\Ground.png");
	Waku1 = new iex2DObj("DATA\\Texture\\Waku1.png");
	Waku2 = new iex2DObj("DATA\\Texture\\Waku2.png");
	StageSelect = new iex2DObj("DATA\\Texture\\stageselect.png");
	White = new iex2DObj("DATA\\Texture\\stagewhite.png");
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
		if (GroundX <= 410)
		{
			GroundX = 410;
			KasenX = 900;
			step = Select;
		}
		break;
	case Select:
		Time++;
		SelectUpdate();
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
		if (KasenX <= -800)
		{
			KasenX = -800;
			GroundX = 1600;

            GameInitializer_UsualMatch::StageType Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
            
            switch (stagetexture)
            {
            case Kasentex:
                Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
                break;
            case Groundtex:
                Stage = GameInitializer_UsualMatch::StageType::School;
                break;
            }

            initparam.pInitializer = new GameInitializer_UsualMatch(Stage, initparam);

			MainFrame->ChangeScene(new sceneGamePlay(initparam));
		}
		break;
	}
	
}
void SceneStageSelect::Render()
{
	Back->Render(0, 0, 1296, 758, 0, 0, 500, 500);//ƒoƒbƒN
	StageSelect->Render(400, TitleY, 512, 128, 0, 0, 512, 128);//
	
	Ground->Render(GroundX+288 - STAGE_POS[0].render_size_x / 2,
		120+288- STAGE_POS[0].render_size_y / 2,
		STAGE_POS[0].render_size_x,
		STAGE_POS[0].render_size_y,
		0, 0, 512, 512,
		RS_COPY,GroundColor);
	Kasen->Render(KasenX + 288 - STAGE_POS[1].render_size_x / 2,
		120 + 288 - STAGE_POS[1].render_size_y / 2,
		STAGE_POS[1].render_size_x,
		STAGE_POS[1].render_size_y,
		0, 0, 512, 512,
		RS_COPY, KasenColor);
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
	switch (stagetexture)
	{
	case Groundtex:
		if (Time % 10 < 5)
		{
			Waku1->Render(GroundX + 288 - STAGE_POS[0].render_size_x / 2,
				120 + 288 - STAGE_POS[0].render_size_y / 2,
				STAGE_POS[0].render_size_x,
				STAGE_POS[0].render_size_y,
				0, 0, 512, 512);
		}
		else
		{
			Waku2->Render(GroundX + 288 - STAGE_POS[0].render_size_x / 2,
				120 + 288 - STAGE_POS[0].render_size_y / 2,
				STAGE_POS[0].render_size_x,
				STAGE_POS[0].render_size_y,
				0, 0, 512, 512);
		}
		break;
	case Kasentex:
		if (Time % 10 < 5)
		{
			Waku1->Render(KasenX + 288 - STAGE_POS[1].render_size_x / 2,
				120 + 288 - STAGE_POS[1].render_size_y / 2,
				STAGE_POS[1].render_size_x,
				STAGE_POS[1].render_size_y,
				0, 0, 512, 512);
		}
		else
		{
			Waku2->Render(KasenX + 288 - STAGE_POS[1].render_size_x / 2,
				120 + 288 - STAGE_POS[1].render_size_y / 2,
				STAGE_POS[1].render_size_x,
				STAGE_POS[1].render_size_y,
				0, 0, 512, 512);
		}
		break;
	}
	
}
void SceneStageSelect::SelectRender()
{
	switch (stagetexture)
	{
		GameInitializer_UsualMatch::StageType Stage;
	case Groundtex:
		Stage = GameInitializer_UsualMatch::StageType::School;
		

		Decide->Render(GroundX + 288 - STAGE_POS[0].render_size_x / 2,
			120 + 288 - STAGE_POS[0].render_size_y / 2,
			480, 480, 0, 0, 512, 512);
		White->Render(698 - 576 / 2,
			120 + 288 - 576 / 2,
			576,
			576,
			0, 0, 512, 512, RS_COPY,
			D3DCOLOR_COLORVALUE(1, 1, 1, WhiteAlpha), 0);
		break;
	case Kasentex:
		Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
		Decide->Render(KasenX + 288 - STAGE_POS[1].render_size_x / 2,
			120 + 288 - STAGE_POS[1].render_size_y / 2,
			480, 480, 0, 0, 512, 512);
		White->Render(698 - 576 / 2,
			120 + 288 - 576 / 2,
			576,
			576,
			0, 0, 512, 512, RS_COPY,
			D3DCOLOR_COLORVALUE(1, 1, 1, WhiteAlpha), 0);
		break;

	}
	
}
void SceneStageSelect::SelectUpdate()
{
	switch (stagetexture)
	{
	case Groundtex:
		STAGE_POS[0].render_size_x = (int)((float)STAGE_POS[0].render_size_x* 1.1f);
		STAGE_POS[0].render_size_y = (int)((float)STAGE_POS[0].render_size_y* 1.1f);

		if (STAGE_POS[0].render_size_x >= 576)
			STAGE_POS[0].render_size_x = 576;
		if (STAGE_POS[0].render_size_y >= 576)
			STAGE_POS[0].render_size_y = 576;
		STAGE_POS[1].render_size_x = (int)((float)STAGE_POS[1].render_size_x* 0.9f);
		STAGE_POS[1].render_size_y = (int)((float)STAGE_POS[1].render_size_y* 0.9f);

		if (STAGE_POS[1].render_size_x <= 200)
			STAGE_POS[1].render_size_x = 200;
		if (STAGE_POS[1].render_size_y <= 200)
			STAGE_POS[1].render_size_y = 200;
		KasenX += 50;
		GroundX += 50;
		if (KasenX >= 900)
		{
			KasenX = 900;
			GroundX = 410;
		}
		
		KasenColor = 0xAAAAAAFF;
		GroundColor = 0xFFFFFFFF;
		break;
	case Kasentex:
		STAGE_POS[1].render_size_x = (int)((float)STAGE_POS[1].render_size_x* 1.1f);
		STAGE_POS[1].render_size_y = (int)((float)STAGE_POS[1].render_size_y* 1.1f);

		if (STAGE_POS[1].render_size_x >= 576)
			STAGE_POS[1].render_size_x = 576;
		if (STAGE_POS[1].render_size_y >= 576)
			STAGE_POS[1].render_size_y = 576;
		STAGE_POS[0].render_size_x = (int)((float)STAGE_POS[0].render_size_x* 0.9f);
		STAGE_POS[0].render_size_y = (int)((float)STAGE_POS[0].render_size_y* 0.9f);

		if (STAGE_POS[0].render_size_x <= 200)
			STAGE_POS[0].render_size_x = 200;
		if (STAGE_POS[0].render_size_y <= 200)
			STAGE_POS[0].render_size_y = 200;

		KasenX -= 50;
		GroundX -= 50;
		if (KasenX <= 410)
		{
			KasenX = 410;
			GroundX = -80;
		}
		KasenColor = 0xFFFFFFFF;
		GroundColor = 0xAAAAAAFF;
		break;
	}


}
void SceneStageSelect::WakuUpdate()
{
	switch (stagetexture)
	{
	case Groundtex:
		if (controller::GetTRG(controller::button::right, 0) && GroundX==410)
		{
			WakuX = 700;
			stagetexture = Kasentex;
			
		}
		else if (controller::GetTRG(controller::button::left, 0) && GroundX == 410)
		{
			WakuX = 700;
			stagetexture = Kasentex;
			
		}
		break;
	case Kasentex:
		if (controller::GetTRG(controller::button::right, 0) && KasenX == 410)
		{
			WakuX = 162;
			stagetexture = Groundtex;
			
		}
		else if (controller::GetTRG(controller::button::left, 0) && KasenX == 410)
		{
			WakuX = 162;
			stagetexture = Groundtex;
			
		}
		break;
	}

}

