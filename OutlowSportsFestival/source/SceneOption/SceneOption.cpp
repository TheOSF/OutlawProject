#include "SceneOption.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"
#include "../GameSystem/GameController.h"

#include "../Sound/Sound.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"

#include "../SceneCharacterSelect/SceneCharacterSelect.h"

struct POSITON_SIZE
{
    int x, y, size_x, size_y, render_size_x, render_size_y;
};
static POSITON_SIZE BALLTEXEL[4] =
{
    { 302, 155, 160, 160, 150, 150 },
    { 509, 157, 120, 120, 250, 250 },
    { 659, 160, 140, 140, 150, 150 },
    { 806, 173, 160, 160, 110, 110 }
};

static POSITON_SIZE PLAYER_C_POS[4] =
{
    { 490, 400, 200, 200, 225, 225 },
    { 710, 400, 200, 200, 225, 225 },
    { 950, 400, 200, 200, 225, 225 },
    { 1190, 400, 200, 200, 225, 225 }
};

static POSITON_SIZE TIME_C_POS[3] =
{
    { 1180, 800, 150, 100, 150, 100 },
    { 1410, 800, 150, 100, 150, 100 },
    { 1660, 800, 140, 100, 150, 100 },
};

static POSITON_SIZE ROUND_C_POS[3] =
{
    { 1200, 950, 100, 100, 100, 100 },
    { 1420, 950, 100, 100, 100, 100 },
    { 1610, 950, 100, 100, 100, 100 },
};

SceneOption::SceneOption()
{
    iexLight::SetFog(800, 1000, 0);
    Back = new iex2DObj("DATA\\Texture\\sports.png");
    select = new iex2DObj("DATA\\Texture\\option2.png");
    Topping = new iex2DObj("DATA\\Texture\\topping.png");
    mode = Player;
    player = one;
    time = hunnon;
    round = Roundnon;
    Playernum = 1;
    nagare = Sinkou;
    counter = 0;
    TitleX = 2000;
    PlayerX = 2000;
    TimeX = 2000;
    RoundX = 2000;
    TbarX = 1500;
    TbarX2 = 1800;
    TbarX3 = 2100;
    TbarX4 = 3000;
    TbarX5 = 3300;
    TbarX6 = 3600;
    TballX = -1100;
    TarrowX = -1500;
    TrayX = -1600;
    BallY = -600;
    gra = 0;
    bound_count = 0;
    render_ball_type = tennis;
    BallAlpha = 1.0f;
    ShadowAlpha = .0f;
    RayAlpha = .6f;
    CorsorAlpha = .0f;
    GearAlpha = .0f;
    Ball_Ground_y = 800;
    BallPosInit();
    nextsceneflg = false;

    Sound::StopBGM();
    Sound::Play(Sound::BGM_Option, 1, true);
}

SceneOption::~SceneOption()
{

    delete Back;
    delete select;
    delete Topping;

}
void SceneOption::BallPosInit()
{
    ball_move_y = .0f;
    ball_y = -(float)BALLTEXEL[soccer].render_size_x;
    bound_count = 0;
}
void SceneOption::Switchplayer()
{
    switch (player)
    {
    case one:
        Playernum = 1;
        PLAYER_C_POS[one].render_size_x = (int)((float)PLAYER_C_POS[one].render_size_x*1.05f);
        PLAYER_C_POS[one].render_size_y = (int)((float)PLAYER_C_POS[one].render_size_y*1.05f);

        if (PLAYER_C_POS[one].render_size_x >= 200)
            PLAYER_C_POS[one].render_size_x = 200;
        if (PLAYER_C_POS[one].render_size_y >= 200)
            PLAYER_C_POS[one].render_size_y = 200;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            player = two;
            PLAYER_C_POS[one].render_size_x = 150;
            PLAYER_C_POS[one].render_size_y = 150;
            Sound::Play(Sound::Cursor_move);
        }
        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            player = four;
            PLAYER_C_POS[one].render_size_x = 150;
            PLAYER_C_POS[one].render_size_y = 150;
            Sound::Play(Sound::Cursor_move);
        }
        break;
    case two:
        Playernum = 2;
        PLAYER_C_POS[two].render_size_x = (int)((float)PLAYER_C_POS[two].render_size_x* 1.05f);
        PLAYER_C_POS[two].render_size_y = (int)((float)PLAYER_C_POS[two].render_size_y* 1.05f);

        if (PLAYER_C_POS[two].render_size_x >= 200)
            PLAYER_C_POS[two].render_size_x = 200;
        if (PLAYER_C_POS[two].render_size_y >= 200)
            PLAYER_C_POS[two].render_size_y = 200;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            PLAYER_C_POS[two].render_size_x = 150;
            PLAYER_C_POS[two].render_size_y = 150;
            player = three;
            Sound::Play(Sound::Cursor_move);
        }

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            PLAYER_C_POS[two].render_size_x = 150;
            PLAYER_C_POS[two].render_size_y = 150;
            player = one;
            Sound::Play(Sound::Cursor_move);
        }

        break;
    case three:
        Playernum = 3;

        PLAYER_C_POS[three].render_size_x = (int)((float)PLAYER_C_POS[three].render_size_x* 1.05f);
        PLAYER_C_POS[three].render_size_y = (int)((float)PLAYER_C_POS[three].render_size_y* 1.05f);

        if (PLAYER_C_POS[three].render_size_x >= 200)
            PLAYER_C_POS[three].render_size_x = 200;
        if (PLAYER_C_POS[three].render_size_y >= 200)
            PLAYER_C_POS[three].render_size_y = 200;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            PLAYER_C_POS[three].render_size_x = 150;
            PLAYER_C_POS[three].render_size_y = 150;
            player = four;
            Sound::Play(Sound::Cursor_move);
        }
        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            PLAYER_C_POS[three].render_size_x = 150;
            PLAYER_C_POS[three].render_size_y = 150;
            player = two;
            Sound::Play(Sound::Cursor_move);
        }
        break;
    case four:
        Playernum = 4;

        PLAYER_C_POS[four].render_size_x = (int)((float)PLAYER_C_POS[four].render_size_x * 1.04f);
        PLAYER_C_POS[four].render_size_y = (int)((float)PLAYER_C_POS[four].render_size_y * 1.04f);

        if (PLAYER_C_POS[four].render_size_x >= 200)
            PLAYER_C_POS[four].render_size_x = 200;
        if (PLAYER_C_POS[four].render_size_y >= 200)
            PLAYER_C_POS[four].render_size_y = 200;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            PLAYER_C_POS[four].render_size_x = 150;
            PLAYER_C_POS[four].render_size_y = 150;
            player = three;
            Sound::Play(Sound::Cursor_move);
        }

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            PLAYER_C_POS[four].render_size_x = 150;
            PLAYER_C_POS[four].render_size_y = 150;
            player = one;
            Sound::Play(Sound::Cursor_move);
        }
        break;
    }

}
void SceneOption::Switchtime() {
    switch (time)
    {
    case hun1:
        param.Time = 60 * 60 * 3;

        TIME_C_POS[hun1].render_size_x = (int)((float)TIME_C_POS[hun1].render_size_x* 1.05f);
        TIME_C_POS[hun1].render_size_y = (int)((float)TIME_C_POS[hun1].render_size_y* 1.05f);

        if (TIME_C_POS[hun1].render_size_x >= 200)
            TIME_C_POS[hun1].render_size_x = 200;
        if (TIME_C_POS[hun1].render_size_y >= 100 * 4 / 3)
            TIME_C_POS[hun1].render_size_y = 100 * 4 / 3;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            TIME_C_POS[hun1].render_size_x = 150;
            TIME_C_POS[hun1].render_size_y = 100;
            time = hun2;
            Sound::Play(Sound::Cursor_move);
        }
        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            TIME_C_POS[hun1].render_size_x = 150;
            TIME_C_POS[hun1].render_size_y = 100;
            time = mugen;
            Sound::Play(Sound::Cursor_move);
        }

        break;
    case hun2:
        param.Time = 60 * 60 * 5;

        TIME_C_POS[hun2].render_size_x = (int)((float)TIME_C_POS[hun2].render_size_x* 1.05f);
        TIME_C_POS[hun2].render_size_y = (int)((float)TIME_C_POS[hun2].render_size_y* 1.05f);

        if (TIME_C_POS[hun2].render_size_x >= 200)
            TIME_C_POS[hun2].render_size_x = 200;
        if (TIME_C_POS[hun2].render_size_y >= 100 * 4 / 3)
            TIME_C_POS[hun2].render_size_y = 100 * 4 / 3;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            TIME_C_POS[hun2].render_size_x = 150;
            TIME_C_POS[hun2].render_size_y = 100;
            time = mugen;
            Sound::Play(Sound::Cursor_move);
        }

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            TIME_C_POS[hun2].render_size_x = 150;
            TIME_C_POS[hun2].render_size_y = 100;
            time = hun1;
            Sound::Play(Sound::Cursor_move);
        }
        break;
    case mugen:
        param.Time = INT_MAX;

        TIME_C_POS[mugen].render_size_x = (int)((float)TIME_C_POS[mugen].render_size_x* 1.05f);
        TIME_C_POS[mugen].render_size_y = (int)((float)TIME_C_POS[mugen].render_size_y* 1.05f);

        if (TIME_C_POS[mugen].render_size_x >= 200)
            TIME_C_POS[mugen].render_size_x = 200;
        if (TIME_C_POS[mugen].render_size_y >= 100 * 4 / 3)
            TIME_C_POS[mugen].render_size_y = 100 * 4 / 3;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            TIME_C_POS[mugen].render_size_x = 150;
            TIME_C_POS[mugen].render_size_y = 100;
            time = hun1;
            Sound::Play(Sound::Cursor_move);
        }
        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            TIME_C_POS[mugen].render_size_x = 150;
            TIME_C_POS[mugen].render_size_y = 100;
            time = hun2;
            Sound::Play(Sound::Cursor_move);
        }


        break;
    }

}
void SceneOption::Switchround() {

    switch (round)
    {
    case R1:
        param.Round = 2;

        ROUND_C_POS[R1].render_size_x = (int)((float)ROUND_C_POS[R1].render_size_x* 1.05f);
        ROUND_C_POS[R1].render_size_y = (int)((float)ROUND_C_POS[R1].render_size_y* 1.05f);

        if (ROUND_C_POS[R1].render_size_x >= 150)
            ROUND_C_POS[R1].render_size_x = 150;
        if (ROUND_C_POS[R1].render_size_y >= 150)
            ROUND_C_POS[R1].render_size_y = 150;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            ROUND_C_POS[R1].render_size_x = 100;
            ROUND_C_POS[R1].render_size_y = 100;
            round = R3;
            Sound::Play(Sound::Cursor_move);
        }
        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            ROUND_C_POS[R1].render_size_x = 100;
            ROUND_C_POS[R1].render_size_y = 100;
            round = R5;
            Sound::Play(Sound::Cursor_move);
        }
        break;
    case R3:
        param.Round = 3;

        ROUND_C_POS[R3].render_size_x = (int)((float)ROUND_C_POS[R3].render_size_x* 1.05f);
        ROUND_C_POS[R3].render_size_y = (int)((float)ROUND_C_POS[R3].render_size_y* 1.05f);

        if (ROUND_C_POS[R3].render_size_x >= 150)
            ROUND_C_POS[R3].render_size_x = 150;
        if (ROUND_C_POS[R3].render_size_y >= 150)
            ROUND_C_POS[R3].render_size_y = 150;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            ROUND_C_POS[R3].render_size_x = 100;
            ROUND_C_POS[R3].render_size_y = 100;
            round = R5;
            Sound::Play(Sound::Cursor_move);
        }

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            ROUND_C_POS[R3].render_size_x = 100;
            ROUND_C_POS[R3].render_size_y = 100;
            round = R1;
            Sound::Play(Sound::Cursor_move);
        }

        break;
    case R5:
        param.Round = 4;

        ROUND_C_POS[R5].render_size_x = (int)((float)ROUND_C_POS[R5].render_size_x* 1.05f);
        ROUND_C_POS[R5].render_size_y = (int)((float)ROUND_C_POS[R5].render_size_y* 1.05f);

        if (ROUND_C_POS[R5].render_size_x >= 150)
            ROUND_C_POS[R5].render_size_x = 150;
        if (ROUND_C_POS[R5].render_size_y >= 150)
            ROUND_C_POS[R5].render_size_y = 150;

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_right, 0)) {
            ROUND_C_POS[R5].render_size_x = 100;
            ROUND_C_POS[R5].render_size_y = 100;
            round = R1;
            Sound::Play(Sound::Cursor_move);
        }

        if (GetKeyVec_stick_or_dpad(key_vec_type::vec_left, 0)) {
            ROUND_C_POS[R5].render_size_x = 100;
            ROUND_C_POS[R5].render_size_y = 100;
            round = R3;
            Sound::Play(Sound::Cursor_move);
        }


        break;
    }

}
void SceneOption::Change() {
    switch (mode)
    {
    case Player:
        if (controller::GetTRG(controller::button::maru, 0)) {
            mode = Time;
            time = hun1;
            Sound::Play(Sound::Cursor_enter);
        }
        if (controller::GetTRG(controller::button::batu, 0)) {
            //MainFrame->ChangeScene(new Title());
            Sound::Play(Sound::Cursor_cancel);
        }
        Switchplayer();
        break;
    case Time:
        if (controller::GetTRG(controller::button::maru, 0)) {
            mode = Round;
            round = R1;
            Sound::Play(Sound::Cursor_enter);
        }
        if (controller::GetTRG(controller::button::batu, 0)) {
            mode = Player;
            time = hunnon;
            Sound::Play(Sound::Cursor_cancel);
        }
        Switchtime();
        break;
    case Round:
        if (controller::GetTRG(controller::button::maru, 0)) {
            mode = Decide;
            Sound::Play(Sound::Cursor_enter);
        }
        if (controller::GetTRG(controller::button::batu, 0)) {
            mode = Time;
            round = Roundnon;
            Sound::Play(Sound::Cursor_cancel);
        }
        Switchround();

        break;
    }

}
void SceneOption::CorsorRender() {
    if (player == one)
        select->Render(
        624 - PLAYER_C_POS[one].render_size_x / 2,
        273 - PLAYER_C_POS[one].render_size_y / 2,
        PLAYER_C_POS[one].render_size_x,
        PLAYER_C_POS[one].render_size_y,
        PLAYER_C_POS[one].x,
        PLAYER_C_POS[one].y,
        PLAYER_C_POS[one].size_x,
        PLAYER_C_POS[one].size_y,
        RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));
    if (player != one)
        select->Render(550, 200, 150, 150, 490, 170, 200, 200, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (player == two)
        select->Render(
        806 - PLAYER_C_POS[two].render_size_x / 2,
        272 - PLAYER_C_POS[two].render_size_y / 2,
        PLAYER_C_POS[two].render_size_x,
        PLAYER_C_POS[two].render_size_y,
        PLAYER_C_POS[two].x,
        PLAYER_C_POS[two].y,
        PLAYER_C_POS[two].size_x,
        PLAYER_C_POS[two].size_y);
    if (player != two)
        select->Render(730, 200, 150, 150, 710, 170, 200, 200, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (player == three)
        select->Render(
        986 - PLAYER_C_POS[three].render_size_x / 2,
        272 - PLAYER_C_POS[three].render_size_y / 2,
        PLAYER_C_POS[three].render_size_x,
        PLAYER_C_POS[three].render_size_y,
        PLAYER_C_POS[three].x,
        PLAYER_C_POS[three].y,
        PLAYER_C_POS[three].size_x,
        PLAYER_C_POS[three].size_y);
    if (player != three)
        select->Render(910, 200, 150, 150, 950, 170, 200, 200, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (player == four)
        select->Render(
        1165 - PLAYER_C_POS[four].render_size_x / 2,
        273 - PLAYER_C_POS[four].render_size_y / 2,
        PLAYER_C_POS[four].render_size_x,
        PLAYER_C_POS[four].render_size_y,
        PLAYER_C_POS[four].x,
        PLAYER_C_POS[four].y,
        PLAYER_C_POS[four].size_x,
        PLAYER_C_POS[four].size_y);
    if (player != four)
        select->Render(1090, 200, 150, 150, 1190, 170, 200, 200, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (time == hun1)
        select->Render(
        624 - TIME_C_POS[hun1].render_size_x / 2,
        450 - TIME_C_POS[hun1].render_size_y / 2,
        TIME_C_POS[hun1].render_size_x,
        TIME_C_POS[hun1].render_size_y,
        TIME_C_POS[hun1].x,
        TIME_C_POS[hun1].y,
        TIME_C_POS[hun1].size_x,
        TIME_C_POS[hun1].size_y);
    if (time != hun1)
        select->Render(550, 400, 150, 100, 490, 800, 150, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (time == hun2)
        select->Render(
        884 - TIME_C_POS[hun2].render_size_x / 2,
        450 - TIME_C_POS[hun2].render_size_y / 2,
        TIME_C_POS[hun2].render_size_x,
        TIME_C_POS[hun2].render_size_y,
        TIME_C_POS[hun2].x,
        TIME_C_POS[hun2].y,
        TIME_C_POS[hun2].size_x,
        TIME_C_POS[hun2].size_y);
    if (time != hun2)
        select->Render(800, 400, 150, 100, 710, 800, 150, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (time == mugen)
        select->Render(
        1124 - TIME_C_POS[mugen].render_size_x / 2,
        450 - TIME_C_POS[mugen].render_size_y / 2,
        TIME_C_POS[mugen].render_size_x,
        TIME_C_POS[mugen].render_size_y,
        TIME_C_POS[mugen].x,
        TIME_C_POS[mugen].y,
        TIME_C_POS[mugen].size_x,
        TIME_C_POS[mugen].size_y);
    if (time != mugen)
        select->Render(
        1050, 400, 150, 100,
        970, 800, 140, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (round == R1)
        select->Render(
        624 - ROUND_C_POS[R1].render_size_x / 2,
        595 - ROUND_C_POS[R1].render_size_y / 2,
        ROUND_C_POS[R1].render_size_x,
        ROUND_C_POS[R1].render_size_y,
        ROUND_C_POS[R1].x,
        ROUND_C_POS[R1].y,
        ROUND_C_POS[R1].size_x,
        ROUND_C_POS[R1].size_y);

    if (round != R1)
        select->Render(575, 545, 100, 100, 490, 950, 100, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (round == R3)
        select->Render(
        904 - ROUND_C_POS[R3].render_size_x / 2 - 20,
        595 - ROUND_C_POS[R3].render_size_y / 2,
        ROUND_C_POS[R3].render_size_x,
        ROUND_C_POS[R3].render_size_y,
        ROUND_C_POS[R3].x,
        ROUND_C_POS[R3].y,
        ROUND_C_POS[R3].size_x,
        ROUND_C_POS[R3].size_y);
    if (round != R3)
        select->Render(845 - 20, 545, 100, 100, 700, 950, 100, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

    if (round == R5)
        select->Render(
        1109 - ROUND_C_POS[R5].render_size_x / 2,
        599 - ROUND_C_POS[R5].render_size_y / 2,
        ROUND_C_POS[R5].render_size_x,
        ROUND_C_POS[R5].render_size_y,
        ROUND_C_POS[R5].x,
        ROUND_C_POS[R5].y,
        ROUND_C_POS[R5].size_x,
        ROUND_C_POS[R5].size_y);
    if (round != R5)
        select->Render(1060, 549, 100, 100, 900, 950, 100, 100, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, CorsorAlpha));

}
void SceneOption::BarRender() {
    switch (mode) {
    case Player:
        select->Render(-50, 220, 700, 100, 0, 1100, 1200, 200);
        break;
    case Time:
        select->Render(-50, 405, 700, 100, 0, 1100, 1200, 200);
        break;
    case Round:
        select->Render(-50, 565, 700, 100, 0, 1100, 1200, 200);
        break;
    }
}
void SceneOption::SelectCome() {
    counter++;

    if (counter >= 50)TitleX -= 60;
    if (TitleX <= 100)TitleX = 100;
    if (counter >= 60)PlayerX -= 60;
    if (PlayerX <= 50)PlayerX = 50;
    if (counter >= 70)TimeX -= 60;
    if (TimeX <= 50)TimeX = 50;
    if (counter >= 80)RoundX -= 60;
    if (RoundX <= 50)RoundX = 50;

    if (counter >= 120)nagare = Syuryo;
}
void SceneOption::SelectLeave() {
    counter++;

    if (counter >= 50)TitleX += 60;
    if (TitleX >= 2000)TitleX = 2000;
    if (counter >= 55)PlayerX += 60;
    if (PlayerX >= 2000)PlayerX = 2000;
    if (counter >= 60)TimeX += 60;
    if (TimeX >= 2000)TimeX = 2000;
    if (counter >= 65)RoundX += 60;
    if (RoundX >= 2000)RoundX = 2000;

    if (RoundX >= 2000)nextsceneflg = true;

}
void SceneOption::ToppingUpdate() {

    if (nagare == Syuryo) {
        TbarX -= 1;
        TbarX2 -= 1;
        TbarX3 -= 1;
        TbarX4 -= 1;
        TbarX5 -= 1;
        TbarX6 -= 1;
    }
    else if (nagare == leave) {
        TbarX -= 40;
        TbarX2 -= 40;
        TbarX3 -= 40;
        TbarX4 -= 40;
        TbarX5 -= 40;
        TbarX6 -= 40;
    }
    else {
        TbarX -= 20;
        TbarX2 -= 20;
        TbarX3 -= 20;
        TbarX4 -= 20;
        TbarX5 -= 20;
        TbarX6 -= 20;
    }
    TballX += 3;
    TarrowX += 8;
    TrayX += 4;
    if (nagare != leave) {
        if (TbarX <= -1500)TbarX = 1500;
        if (TbarX2 <= -1500)TbarX2 = 1500;
        if (TbarX3 <= -1500)TbarX3 = 1500;
        if (TbarX4 <= -1500)TbarX4 = 1500;
        if (TbarX5 <= -1500)TbarX5 = 1500;
        if (TbarX6 <= -1500)TbarX6 = 1500;
    }
    if (TballX >= 1500)TballX = -1150;
    if (TarrowX >= 1500)TarrowX = -1550;
    if (TrayX >= 1500)TrayX = -1600;

}
void SceneOption::ToppingRender()
{
    Topping->Render(TbarX, 140, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);
    Topping->Render(TbarX2, 345, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);
    Topping->Render(TbarX3, 480, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);

    Topping->Render(TbarX4, 140, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);
    Topping->Render(TbarX5, 345, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);
    Topping->Render(TbarX6, 480, 1550, 50, 5, 0, 1000, 100, RS_COPY, 0xCCFFFFFF);

    Topping->Render(TballX, 150, 600, 50, 300, 100, 600, 50, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, RayAlpha));
    Topping->Render(TarrowX, 353, 600, 50, 300, 100, 600, 50, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, RayAlpha));
    Topping->Render(TrayX, 490, 600, 50, 300, 100, 600, 50, RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, RayAlpha));

    static float a = .0f;
    if (a > FLT_MAX - 10)a = 0;

    Topping->RenderPlus(Vector2(40, 40), Vector2(700, 700), a += 0.01f, Vector2(0, 170), Vector2(300, 300), RS_COPY,
        D3DCOLOR_COLORVALUE(1, 1, 1, GearAlpha), 0);

}
void SceneOption::BallUpdate()
{
    /*static Vector2 m;
    Vector2 n = Vector2(KEY(KEY_AXISX, 0)*0.001f, KEY(KEY_AXISY, 0)*0.001f);

    if (Vector2Length(m) < 0.5f){

    BALLTEXEL[render_ball_type].x += n.x;
    BALLTEXEL[render_ball_type].y += n.y;
    }

    m = n;*/

    static const float BallGlavity = 0.3f;

    //if (KEY(KEY_ENTER,0)){
    ball_move_y += BallGlavity;
    ball_y += ball_move_y;
    //}

    if (ball_y + BALLTEXEL[render_ball_type].render_size_x > Ball_Ground_y)
    {

        ball_y = (float)(Ball_Ground_y - BALLTEXEL[render_ball_type].render_size_x);

        switch (bound_count)
        {
        case 0:
            ball_move_y = -15;
            break;
        case 3:
            ball_move_y = -25;
            break;
        default:
            ball_move_y = -ball_move_y;
            break;
        }
        bound_count++;
    }
    if (ball_y <= -70.0f&&bound_count >= 3) {
        render_ball_type = (Fallball)(((Fallball)render_ball_type + 1) % 4);
        BallPosInit();
    }
}
void SceneOption::BallRender()
{
    const int BallRender_X = 1100;
    int ballsize = BALLTEXEL[render_ball_type].render_size_x;

    float shadow_size =
        BALLTEXEL[render_ball_type].render_size_x*
        (ball_y / Ball_Ground_y);

    if (shadow_size > 0)
    {
        Topping->Render(
            BallRender_X - (int)(shadow_size / 2),
            Ball_Ground_y - (int)((shadow_size + 170) / 2),
            (int)(shadow_size + 30),
            (int)(shadow_size / 2),
            450, 330, 180, 60,
            RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, ShadowAlpha)
            );
    }

    Topping->Render(
        BallRender_X - ballsize / 2,
        (int)ball_y - ballsize / 2,
        ballsize,
        ballsize,
        BALLTEXEL[render_ball_type].x,
        BALLTEXEL[render_ball_type].y,
        BALLTEXEL[render_ball_type].size_x,
        BALLTEXEL[render_ball_type].size_y,
        RS_COPY, D3DCOLOR_COLORVALUE(1, 1, 1, BallAlpha)
        );
}

void SceneOption::Update()
{
    //Sound::Play(Sound::Cursor_enter);
    ToppingUpdate();
    switch (nagare) {
    case Sinkou:
        SelectCome();
        break;
    case Syuryo:
        CorsorAlpha += 0.04f;
        if (CorsorAlpha >= 1.0f)CorsorAlpha = 1.0f;
        GearAlpha += 0.01f;
        if (GearAlpha >= .8f)GearAlpha = .8f;
        ShadowAlpha = ball_y / (float)Ball_Ground_y;
        BallUpdate();
        Change();
        if (controller::GetTRG(controller::button::maru, 0)
            && mode == Decide) {
            nagare = leave;
            counter = 0;
            Sound::Play(Sound::Scene_Enter);
        }
        break;
    case leave:
        SelectLeave();
        BallAlpha -= 0.05f;
        GearAlpha -= 0.01f;
        RayAlpha -= 0.01f;
        if (RayAlpha <= .0f)RayAlpha = .0f;
        if (GearAlpha <= .0f)GearAlpha = .0f;
        if (BallAlpha <= .0f)BallAlpha = .0f;
        ShadowAlpha = .0f;
        BallUpdate();

        if (nextsceneflg)
        {
            MainFrame->ChangeScene(new SceneCharacterSelect(param, (int)player + 1));
        }
        break;
    }
}
void SceneOption::Render()
{
    Back->Render(0, 0, 1944, 1137, 0, 0, 500, 500);//バック
    ToppingRender();
    if (nagare == Syuryo) {

        BarRender();
    }


    BallRender();
    select->Render(TitleX, 50, 600, 150, 0, 0, 600, 150);//タイトル
    select->Render(PlayerX, 200, 450, 100, 0, 170, 450, 100);
    select->Render(TimeX, 400, 450, 100, 0, 800, 450, 100);
    select->Render(RoundX, 550, 450, 100, 0, 960, 450, 100);

    //select->Render(TitleX, 50, 900, 225, 0, 0, 600, 150);//タイトル
    //select->Render(PlayerX, 300, 675, 150, 0, 170, 450, 100);
    //select->Render(TimeX, 500, 675, 150, 0, 800, 450, 100);
    //select->Render(RoundX, 650, 675, 150, 0, 960, 450, 100);

    if (nagare == Syuryo)CorsorRender();
}
bool SceneOption::GetKeyVec_stick_or_dpad(key_vec_type::KEY_VEC_TYPE type, int number)
{
    using namespace key_vec_type;

    switch (type)
    {
    case vec_up:
        /*if (controller::GetStickValue(controller::stick::left, number) ||
        controller::GetTRG(controller::button::shikaku,number))return true;*/
        if (controller::GetTRG(controller::button::up, number))
            return true;
        break;
    case vec_down:
        /*if (SelectFunction::GetStickValue(type, number) ||
        KEY(KEY_DOWN, number) == 3)return true;*/
        if (controller::GetTRG(controller::button::down, number))
            return true;
        break;
    case vec_right:
        /*if (SelectFunction::GetStickValue(type, number) ||
        KEY(KEY_RIGHT, number) == 3)return true;*/
        if (controller::GetTRG(controller::button::right, number))
            return true;
        break;
    case vec_left:
        /*if (SelectFunction::GetStickValue(type, number) ||
        KEY(KEY_LEFT, number) == 3)return true;*/
        if (controller::GetTRG(controller::button::left, number))
            return true;
        break;
    default:
        MyAssert(false, "存在しないタイプのキー方向が指定されました");
        break;
    }
    return false;
}

//void SceneOption::Update()
//{
//
//    if (true)
//    {
//        sceneGamePlay::InitParams param;
//
//        GameInitializer_UsualMatch::StageType Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
//        const UINT Round = 3;
//        const UINT Frame = 60 * 60 * 2;
//
//        param.Round = Round;
//        param.Time = Frame;
//        param.PlayerArray.fill({ -1, PlayerType::_Player, CharacterType::_Tennis, StrongType::_Strong });
//
//        //キャラ選択
//
//        param.PlayerArray.at(0) = { 0, PlayerType::_Computer, CharacterType::_Soccer,StrongType::_Strong };
//        param.PlayerArray.at(1) = { 1, PlayerType::_Computer, CharacterType::_Soccer,StrongType::_Strong };
//        param.PlayerArray.at(2) = { 2, PlayerType::_Computer, CharacterType::_Soccer,StrongType::_Strong };
//        param.PlayerArray.at(3) = { 3, PlayerType::_Computer, CharacterType::_Soccer,StrongType::_Strong };
//
//
//        param.pInitializer = new GameInitializer_UsualMatch(Stage, param);
//        MainFrame->ChangeScene(new sceneGamePlay(param));
//    }
//
//}