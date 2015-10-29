#include "AnimationBordRenderer.h"
#include "../Camera/Camera.h"


AnimationBordRenderer::AnimationBordRenderer(
    LPIEX2DOBJ      pTexture,
    int             NumCellX,
    int             NumCellY,
    int             MaxCell,
    COLORf          Color,
    DWORD           hdr
    ) :
    m_pTexture(pTexture),
    m_NumCellX(NumCellX),
    m_NumCellY(NumCellY),
    m_MaxCell(MaxCell),
    m_Color(Color.toDWORD()),
    m_HdrCol(hdr)
{
    m_Pos = Vector3Zero;

    m_Right = Vector3AxisX;
    m_Up = Vector3AxisY;

    m_Size = Vector2(1, 1);

    m_CellCount = 0;
}


AnimationBordRenderer::~AnimationBordRenderer()
{

}

int AnimationBordRenderer::GetMaxCell()const
{
    return m_MaxCell;
}

void AnimationBordRenderer::CalcZ()
{
    m_SortZ = DefCamera.GetCameraZ(m_Pos);
}

void AnimationBordRenderer::Render()
{
    static HdrLVERTEX v[4];

    v[0].pos = m_Right*-m_Size.x + m_Up*m_Size.y;
    v[1].pos = m_Right* m_Size.x + m_Up*m_Size.y;
    v[2].pos = m_Right*-m_Size.x - m_Up*m_Size.y;
    v[3].pos = m_Right* m_Size.x - m_Up*m_Size.y;

    for (int i = 0; i < 4; ++i)
    {
        v[i].color = m_Color;
        v[i].HdrColor = m_HdrCol;

        v[i].pos.x += m_Pos.x;
        v[i].pos.y += m_Pos.y;
        v[i].pos.z += m_Pos.z;
    }

    const Vector2 CellSize(1 / (float)m_NumCellX, 1 / (float)m_NumCellY);

    v[0].tu = (float)(m_CellCount % m_NumCellX)*CellSize.x;
    v[0].tv = (float)(m_CellCount / m_NumCellX)*CellSize.y;

    v[1].tu = v[0].tu + CellSize.x;
    v[1].tv = v[0].tv;

    v[2].tu = v[0].tu;
    v[2].tv = v[0].tv + CellSize.y;

    v[3].tu = v[1].tu;
    v[3].tv = v[2].tv;


    iexPolygon::PolygonRender3DHDR(
        v, 2, m_pTexture, shader, "HDR_ForwardPolygon"
        );

}



//---------------------------------------------------------
//　板ポリゴンアニメーション描画クラスを動かすクラス
//---------------------------------------------------------

AnimationBordGameObj::AnimationBordGameObj(
    AnimationBordRenderer*   pRenderer,        //描画クラスへのポインタ(終了時にdeleteする)
    int     live_frame   //生存時間(デフォルトでは無限)
    ) :
    m_pRenderer(pRenderer),

    move_speed(Vector3Zero),
    move_power(Vector3Zero),

    rotate_speed(Vector3Zero),
    rotate_power(Vector3Zero),

    scale_speed(0, 0),
    scale_power(0, 0),

    animation_speed(1),

    animation_end_delete(true),

    animation_loop(false),

    m_LiveFrame(live_frame),
    m_MyCellCount(0.0f)
{
    m_MyCellCount = (float)m_pRenderer->m_CellCount;
}


AnimationBordGameObj::~AnimationBordGameObj()
{
    delete m_pRenderer;
}


bool AnimationBordGameObj::Update()
{
    //フレーム更新
    UpdateFrame();


    //回転の更新
    UpdateRotation();

    //移動の更新
    UpdatePosition();

    //スケールの更新
    UpdateScale();


    const bool isDelete = 
        (animation_end_delete && m_pRenderer->GetMaxCell() <= m_pRenderer->m_CellCount) ||
        (m_LiveFrame != m_Infinite_Livetime && m_LiveFrame <= 0);

    return !isDelete;
}

bool AnimationBordGameObj::Msg(MsgType mt)
{
    return false;
}


void AnimationBordGameObj::UpdateFrame()
{

    //アニメーションコマの更新
    m_MyCellCount += animation_speed;
    m_pRenderer->m_CellCount = (int)m_MyCellCount;


    //生存時間の更新
    if (m_LiveFrame != m_Infinite_Livetime)
    {
        --m_LiveFrame;
    }
}


void AnimationBordGameObj::UpdateRotation()
{

    if (rotate_speed == Vector3Zero)
    {
        D3DXQUATERNION q;

        D3DXQuaternionRotationYawPitchRoll(&q, rotate_speed.y, rotate_speed.x, rotate_speed.z);

        m_pRenderer->m_Right = Vector3RotateQuaternion(q, m_pRenderer->m_Right);
        m_pRenderer->m_Up = Vector3RotateQuaternion(q, m_pRenderer->m_Up);
    }

    rotate_speed += rotate_power;
}

void AnimationBordGameObj::UpdateScale()
{
    m_pRenderer->m_Size += scale_speed;
    scale_speed += scale_power;
}

void AnimationBordGameObj::UpdatePosition()
{
    m_pRenderer->m_Pos += move_speed;
    move_speed += move_power;
}