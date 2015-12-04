#include "Item_WallBoundBall.h"
#include "../character/CharacterManager.h"
#include "../GameSystem/ResourceManager.h"
#include "../Collision/Collision.h"
#include "../Camera/Camera.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/GlavityLocus.h"

Item_WallBoundBall::Item_WallBoundBall(
    CrVector3 pos,
    CrVector3 move,
    CrVector3 target) :
    m_Locus(28),
    m_LiveFlg(true),
    m_Target(target),
    m_ColorTimer(0),
    m_Timer(0),
    m_Glavity(-0.03f),
    m_MeshRenderer(DefResource.Get(Resource::MeshType::Sphere), false, MeshRenderer::RenderType::NoTexture),
    m_pStateFunc(&Item_WallBoundBall::State_ToFloorMove)
{

    //ダメージ初期化(ダメージいるのか？
    {
        m_Damage.MaxChrHit = 0;  //キャラクタには当たらない
        m_Damage.m_VecType = DamageShpere::DamageVecType::MemberParam;
        m_Damage.pBall = this;
        m_Damage.m_Enable = false;
    }

    //軌跡
    {
        m_Locus.m_StartParam.Width=0.5f;
        m_Locus.m_EndParam.Width = 0.1f;
    }


    //ボールパラメータ初期化
    {
        m_Params.pos = pos;
        m_Params.move = move;
        m_Params.pParent = DefCharacterMgr.GetCharacterMap().begin()->first;
        m_Params.scale = 0.5f;
        m_Params.type = BallBase::Type::_Usual;
    }
}

Item_WallBoundBall::~Item_WallBoundBall()
{

}


bool Item_WallBoundBall::Update()
{
    //更新
    (this->*m_pStateFunc)();


    //軌跡の点を追加
    {
        Vector3 vec;

        Vector3Cross(vec, m_Params.move, DefCamera.GetForward());

        if (vec == Vector3Zero)
        {
            vec = Vector3AxisX;
        }
        else
        {
            vec.Normalize();
        }
        
        m_Locus.AddPoint(
            m_Params.pos,
            vec
            );
    }

    //ダメージ更新
    UpdateDamageClass();

    //メッシュ更新
    UpdateMesh();

    //軌跡
    UpdateLocusColor();

    //色たいまー
    m_ColorTimer += 0.1f;

    //下すぎたら消す
    if (m_LiveFlg && m_Params.pos.Length() > 150.0f)
    {
        m_LiveFlg = false;
    }

    return m_LiveFlg;
}

bool Item_WallBoundBall::Msg(MsgType mt)
{
    if (mt == MsgType::_GameSet)
    {
        
    }

    return false;
}

Vector4 Item_WallBoundBall::GetColor(RATIO t)const
{
    Vector4 ret, col1, col2;

    //0＝赤 0.25=緑 0.5 = 青 0.75 =黄色 1.0=水色

    if (t <= 0.25f)
    {
        col1 = Vector4(1, 0, 0, 1);
        col2 = Vector4(0, 1, 0, 1);

        t /= 0.25f;
    }
    else if (t <= 0.5f)
    {
        col1 = Vector4(0, 1, 0, 1);
        col2 = Vector4(0, 0, 1, 1);

        t -= 0.25f;
        t /= 0.25f;
    }
    else if (t <= 0.75f)
    {
        col1 = Vector4(0, 0, 1, 1);
        col2 = Vector4(1, 1, 0, 1);

        t -= 0.5f;
        t /= 0.25f;
    }
    else
    {
        col1 = Vector4(1, 1, 0, 1);
        col2 = Vector4(0, 1, 1, 1);

        t -= 0.75f;
        t /= 0.25f;
    }
    
    D3DXVec4Lerp(
        &ret,
        &col1,
        &col2,
        t
        );

    return ret;
}

void Item_WallBoundBall::UpdateLocusColor()
{
    Vector4 color = GetColor(sinf(m_ColorTimer)*0.5f + 0.5f);

    m_Locus.m_StartParam.Color = color;
    m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.7f);

    color = GetColor(sinf(m_ColorTimer + PI*0.5f)*0.5f + 0.5f);
    color.w = 0.0f;

    m_Locus.m_EndParam.Color = color;
    m_Locus.m_EndParam.HDRColor = Vector4(1, 1, 1, 0.0f);
}

void Item_WallBoundBall::ApperEffect()
{
    //エフェクト
    Vector3 move(0,0.02f, 0);

    //煙
    for (int i = 0; i < 8; ++i)
    {
        EffectFactory::Smoke(
            m_Params.pos + Vector3Rand(),
            move,
            3.0f,
            0.3f
            );
    }

    Vector3  color(1.0f, 0.5f, 0);
    Vector3 power(0, -0.02f, 0);
    GlavityLocus* g;

    const Vector4
        stCol(color.x, color.y, color.z, 1.0f),
        endCol(color.x, color.y, color.z, 0.5f);

    const Vector4
        stHdCol(color.x, color.y, color.z, 1.0f),
        endHdCol(color.x, color.y, color.z, 0.5f);

    //軌跡(火花
    for (int i = 0; i < 10; ++i)
    {
        move = Vector3Rand();
        move *= frand();
        move.y *= 2.0f;
        move *= 0.75f;

        g = new GlavityLocus(
            m_Params.pos, move, power, 8, 40 + rand() % 20
            );

        g->m_BoundRatio = 0.5f;
        g->m_CheckWall = false;

        g->m_Locus.m_StartParam.Color = stCol;
        g->m_Locus.m_EndParam.Color = endCol;

        g->m_Locus.m_StartParam.HDRColor = stHdCol;
        g->m_Locus.m_EndParam.HDRColor = endHdCol;

        g->m_Locus.m_StartParam.Width = 0.09f;
        g->m_Locus.m_EndParam.Width = 0.00f;

        // g->m_Locus.m_pTexture = DefResource.Get(Resource::TextureType::Locus1);
    }
}

void Item_WallBoundBall::UpdateDamageClass()
{
    m_Damage.m_Param.pos = m_Params.pos;
    m_Damage.m_Param.size = 1.0f;
    m_Damage.m_Vec = m_Params.move;
}

void Item_WallBoundBall::UpdateMesh()
{
    const float Scale = 0.4f;
    Matrix M;

    D3DXMatrixScaling(&M, Scale, Scale, Scale);

    {
        M._41 = m_Params.pos.x;
        M._42 = m_Params.pos.y;
        M._43 = m_Params.pos.z;
    }

    m_MeshRenderer.SetMatrix(M);
}

bool Item_WallBoundBall::WallCheck(Vector3* pNormal)
{
    Vector3 out, pos(m_Params.pos), vec(Vector3Normalize(m_Params.move));
    float Dist = m_Params.move.Length()*1.1f;
    int Material;

    //壁との判定チェック
    if (DefCollisionMgr.RayPick(&out, &pos, &vec, &Dist, &Material, CollisionManager::RayType::_Character) != nullptr)
    {
        *pNormal = vec;
        return true;
    }

    return false;
}

void Item_WallBoundBall::State_ToFloorMove()
{
    Vector3 Normal(0, 0, 0);

    //だんだん位置をあわせる
    m_Params.pos.y += (UsualBallShotY - m_Params.pos.y)*0.2f;

    //位置更新
    //m_Pos.x += m_Move.x;
    //m_Pos.z += m_Move.z;

    //壁判定
    if (WallCheck(&Normal))
    {
        //跳ね返る
        Vector3 NewMove = Vector3Refrect(m_Params.move, Normal);

        m_Params.move.x = NewMove.x;
        m_Params.move.z = NewMove.z;
    }

    //だいたい高さがあっていたら
    if (fabsf(UsualBallShotY - m_Params.pos.y) < 0.2f)
    {
        m_Params.pos.y = UsualBallShotY;
        m_pStateFunc = &Item_WallBoundBall::State_RefrectMove;
    }
}

void Item_WallBoundBall::State_RefrectMove()
{
    Vector3 Normal(0, 0, 0);

    //移動
    m_Params.pos += m_Params.move;

    //壁判定
    if (WallCheck(&Normal))
    {
        //跳ね返る
        m_Params.move = Vector3Refrect(m_Params.move, Normal);
    }
}

void Item_WallBoundBall::State_ToBellMove()
{
    m_Params.move.y += m_Glavity;
    m_Params.pos += m_Params.move;

    //鐘に当たっていた場合
    if (m_Damage.HitCount)
    {
        //エフェクト
        ApperEffect();

        //終了ステートへ
        m_pStateFunc = &Item_WallBoundBall::State_Finish;
    }
}

void Item_WallBoundBall::State_Finish()
{
    m_MeshRenderer.m_Visible = false;

    if (++m_Timer > 30)
    {
        m_LiveFlg = false;
    }
}

void Item_WallBoundBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Params.move = GetToMoveValue();
    m_pStateFunc = &Item_WallBoundBall::State_ToBellMove;

    m_Damage.m_Enable = true;
    m_Damage.pParent = pCounterCharacter;
}


Vector3 Item_WallBoundBall::GetToMoveValue()const
{
    const float touch_frame = 50;

    Vector3 ret;
    Vector3 target = m_Target;

    target.y += (-m_Glavity / 2)*(touch_frame*touch_frame);
    ret = target - m_Params.pos;

    const float len = Vector3Length(ret);
    float v = len / touch_frame;

    ret.Normalize();
    ret *= v;

    return ret;
}