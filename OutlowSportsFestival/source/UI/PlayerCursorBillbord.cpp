#include "PlayerCursorBillbord.h"
#include "../GameSystem/ResourceManager.h"
#include "../character/CharacterFunction.h"
#include "../Camera/Camera.h"


PlayerCursorBillbord::PlayerCursorBillbord(
    CharacterBase*  pCharacter
    ) :
    m_pCharacter(pCharacter),
    m_pCursorTex(DefResource.Get(Resource::TextureType::UI_player_name)),
    m_Alpha(1),
    m_LightUpFrameCount(0)
{
    const int draw_scale = 3;

    //描画座標設定
    m_DrawRect.x = 0;
    m_DrawRect.y = 0;
    m_DrawRect.w = draw_scale * 16;
    m_DrawRect.h = draw_scale * 19;

    //テクスチャ座標設定
    m_TextureRect.x = (int)m_pCharacter->m_PlayerInfo.number * 150 + 30;
    m_TextureRect.y = (int)40;

    m_TextureRect.w = 160;
    m_TextureRect.h = 190;

    if (m_pCharacter->m_PlayerInfo.player_type == PlayerType::_Computer)
    {
        m_TextureRect.x = 650;
    }
}


PlayerCursorBillbord::~PlayerCursorBillbord()
{

}

void PlayerCursorBillbord::LightUp(UINT Frame)
{
    m_LightUpFrameCount = Frame;
}

bool PlayerCursorBillbord::Update()
{
    //フレームカウント
    if (m_LightUpFrameCount > 0)
    {
        --m_LightUpFrameCount;
    }

    return true;
}

bool PlayerCursorBillbord::Msg(MsgType mt)
{
    return false;
}

void PlayerCursorBillbord::CalcZ()
{
    m_SortZ = 0.0f;
}

void PlayerCursorBillbord::Render()
{
    float draw_alpha = 0;

    //死んでいたらAlphaを減少させる
    if (chr_func::isDie(m_pCharacter))
    {
        m_Alpha = max(m_Alpha - 0.02f, 0);
    }
    else
    {
        m_Alpha = 0.8f;
    }

    draw_alpha = m_Alpha;


    /*
    //カメラから近い場合は描画しない
    {
        const float startZ = 50.0f;
        const float endZ = 45.0f;

        float z = DefCamera.GetCameraZ(m_pCharacter->m_Params.pos);

        MyDebugString("z=%f \n", z);

        z -= endZ;
        z /= startZ - endZ;

        z = fClamp(z, 1, 0);

        draw_alpha *= z;
    }
    */


    //見えないなら描画しない
    if (draw_alpha <= 0)
    {
        return;
    }


    //位置を設定
    {
        Vector3 pos;

        if (DefCamera.WorldToProjection(
            &pos,
            m_pCharacter->m_Params.pos + Vector3(0, 7.0f, 0)
            ) == false)
        {
            return;
        }

        m_DrawRect.x = (int)((pos.x* 0.5f + 0.5f) *(float)iexSystem::ScreenWidth);
        m_DrawRect.y = (int)((pos.y*-0.5f + 0.5f)*(float)iexSystem::ScreenHeight);
    }

    {
        RectI rc = m_DrawRect;

        rc.x -= rc.w / 2;
        rc.y -= rc.h / 2;

        //描画
        m_pCursorTex->Render(
            rc,
            m_TextureRect,
            RS_COPY,
            D3DCOLOR_COLORVALUE(1, 1, 1, draw_alpha)
            );

        //点滅
        if (m_LightUpFrameCount % 8 >= 4)
        {
            //描画
            for (int i = 0; i < 5; ++i)
            {
                m_pCursorTex->Render(
                    rc,
                    m_TextureRect,
                    RS_ADD,
                    D3DCOLOR_COLORVALUE(1, 1, 1, 1)
                    );
            }
        }
    }
}