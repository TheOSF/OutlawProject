//*****************************************************************************************************************************
//
//		オプションシーン
//
//*****************************************************************************************************************************
#pragma once
#include "../IexSystem/Scene.h"
#include "../utillity/ScreenEffect.h"
#include "../GameScene/SceneGamePlay.h"
using namespace std;

enum {
	Player,
	Time,
	Round,
	Decide
};
namespace key_vec_type {
	enum KEY_VEC_TYPE
	{
		vec_up,
		vec_down,
		vec_right,
		vec_left
	};
}
class SceneOption : public Scene
{
public:
    class Renderer
    {
    public:
        enum class DrawType
        {
            
        };

        void Render(

            );
    };
	enum Nagare {
		Sinkou,
		Syuryo,
		leave
	};

	enum Fallball {
		tennis,
		soccer,
		baseball,
		golf
	};
	enum PLAYER_num {
		one,
		two,
		three,
		four
	};
	enum TIME_num {
		hun1,
		hun2,
		mugen,
		hunnon,
	};
	enum ROUND_num {
		R1,
		R3,
		R5,
		Roundnon,
	};
	
    SceneOption();
    ~SceneOption();

	iex2DObj* Back;//タイトル画像
	iex2DObj* select;
	iex2DObj* Topping;
	void Change();
	void Switchplayer();
	void Switchround();
	void Switchtime();
	//void CorsorUpdate();
	bool GetKeyVec_stick_or_dpad(key_vec_type::KEY_VEC_TYPE type, int number);
	void CorsorRender();
	void SelectCome();
	void SelectLeave();
	void ToppingRender();
	void ToppingUpdate();
	void BallRender();
	void BallUpdate();
	void BarRender();
	void BallPosInit();
	sceneGamePlay::InitParams param;
	Nagare nagare;
	float BallAlpha;
	float RayAlpha;
	float ShadowAlpha;
	float CorsorAlpha;
	float GearAlpha;
	int Playernum;
	int Ball_Ground_y;
	bool nextsceneflg;
public:
	int mode;
	int player;
	int time;
	int round;
	int counter;
	int TitleX;
	int PlayerX;
	int TimeX;
	int RoundX;
	int TbarX;
	int TbarX2;
	int TbarX3;
	int TbarX4;
	int TbarX5;
	int TbarX6;
	int TballX;
	int TarrowX;
	int TrayX;
	int gra;
	int BallY;

	float ball_y, ball_move_y;
	int bound_count;
	Fallball render_ball_type;

	
	int GetPlayernum() { return Playernum; }


    void Update();
    void Render();
};