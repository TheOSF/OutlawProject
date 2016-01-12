#include "RainbowSkillBall.h"
#include "../GameSystem/ResourceManager.h"
#include "../Effect/EffectFactory.h"
#include "../Effect/ImpactLightObject.h"
#include "../Effect/BlurImpact.h"
#include "../Sound/Sound.h"
#include "../character/CharacterFunction.h"

RainbowSkillBall::RainbowSkillBall(
    CrVector3 pos,
    CrVector3 move,
    CharacterBase* target) :
    m_MeshRenderer(DefResource.Get(Resource::MeshType::Sphere), false, MeshRenderer::RenderType::NoTexture),
    m_Locus(25),
    m_Target(target),
    m_Pos(pos),
    m_Move(move),
    m_HomingRatio(0)
{
    //軌跡
    {
        m_Locus.m_StartParam.Width = 0.5f;
        m_Locus.m_EndParam.Width = 0.1f;
    }

    {
        Vector4 color(1, 0.5f, 0, 1);

        m_Locus.m_StartParam.Color = color;
        m_Locus.m_StartParam.HDRColor = Vector4(1, 1, 1, 0.7f);

        color.w = 0.0f;

        m_Locus.m_EndParam.Color = color;
        m_Locus.m_EndParam.HDRColor = Vector4(1, 1, 1, 0.0f);
    }
}


RainbowSkillBall::~RainbowSkillBall()
{

}

bool RainbowSkillBall::Update()
{
    bool isLive = Move();

    //軌跡の点を追加
    {
        Vector3 vec;

        Vector3Cross(vec, m_Move, DefCamera.GetForward());

        if (vec == Vector3Zero)
        {
            vec = Vector3AxisX;
        }
        else
        {
            vec.Normalize();
        }

        m_Locus.AddPoint(
            m_Pos,
            vec
            );

    }

    //メッシュ更新
    UpdateMesh();

    //エフェクト
    if (isLive == false)
    {
        Effect();
    }

    return isLive;
}

bool RainbowSkillBall::Msg(MsgType mt)
{
    return false;
}

//メッシュ更新
void RainbowSkillBall::UpdateMesh()
{

    const float Scale = 0.4f;
    Matrix M;

    D3DXMatrixScaling(&M, Scale, Scale, Scale);

    {
        M._41 = m_Pos.x;
        M._42 = m_Pos.y;
        M._43 = m_Pos.z;
    }

    m_MeshRenderer.SetMatrix(M);
}

bool RainbowSkillBall::Move()
{
    const Vector3 MoveTarget = m_Target->m_Params.pos + Vector3(0, 2, 0);

    //移動ベクトルを回転
    {
        Vector3 Axis, TargetMoveVec;
        
        TargetMoveVec = MoveTarget - m_Pos;
        Vector3Cross(Axis, m_Move, TargetMoveVec);

        Axis.Normalize();

        RADIAN angle = Vector3Radian(TargetMoveVec, m_Move);

        m_Move = Vector3RotateAxis(Axis, angle * m_HomingRatio, m_Move);

        m_HomingRatio += (1 - m_HomingRatio)*0.05f;
    }

    {
        const float SppedUp = 0.05f;
        
        m_Move = Vector3Normalize(m_Move)*(m_Move.Length() + SppedUp);
    }

    m_Pos += m_Move;

    const float Len = Vector3Distance(m_Pos, MoveTarget);

    {
        const float AlphaLen = 8.0f;

        m_Locus.m_StartParam.HDRColor.w = min(Len / AlphaLen, 1);

    }

    return  Len> 2.0f;
}

void RainbowSkillBall::Effect()
{
    //エフェクトを出して
    new ImpactLightObject(
        m_Pos,
        25.0f,
        Vector3(1, 0.5f, 0),
        0.1f
        );

    new BlurImpactSphere(
        m_Pos,
        15.0f,
        80.0f,
        20
        );

    //ＳＥを鳴らして
    Sound::Play(Sound::Change);

    //ゲージを上昇
    chr_func::AddSkillGauge(m_Target, 1.0f);

    //コントローラを振動
    chr_func::SetControllerShock(
        m_Target,
        1.0f,
        0.15f
        );
}