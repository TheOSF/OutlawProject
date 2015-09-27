#include "ParticleMoveObject.h"

ParticleMoveObject::ParticleMoveObject(
    ParticleBase* pParticle,
    CrVector3     move,
    CrVector3     power,
    UINT          live_time,
    bool          animation,
    UINT          cellX,
    UINT          cellY
    ) :
    pParticle(pParticle),
    move(move),
    power(power),
    delete_time(live_time),
    time(0),
    start_color(pParticle->m_Param.color),
    animation(animation),
    cellX(cellX),
    cellY(cellY)
{
    //一度更新
    Update();

    //増えた分１フレーム戻す
    --time;
}


ParticleMoveObject::~ParticleMoveObject()
{
    delete pParticle;
}


bool ParticleMoveObject::Update()
{
    //移動
    pParticle->m_Param.pos += move;
    move += power;

    //色の補間
    {
        COLORf end_color = start_color;
        end_color.a = 0;
        pParticle->m_Param.color = COLORf::Lerp(start_color, end_color, (float)time / (float)delete_time);
    }
    //テクスチャアニメーション
    if (animation)
    {
        pParticle->SetCellUV((int)cellX, (int)cellY, (int)time);
    }

    //生存カウントorフィールドから離れすぎていると消える
    return
        delete_time > ++time &&
        pParticle->m_Param.pos.Length() < 150;
}


bool ParticleMoveObject::Msg(MsgType mt)
{
    return false;
}



//-----------------------------------------------------------
//      色の管理ができるver
//-----------------------------------------------------------

ParticleMoveObject2::ParticleMoveObject2(
    ParticleBase* pParticle,   //動かすパーティクルへのポインタ
    CrVector3     move,        //移動量
    CrVector3     power,       //力場(重力とか)
    bool          animation,   //テクスチャアニメーションをするかどうか
    UINT          cellX,       //テクスチャのコマ数横(アニメーションしない場合は１を設定してください)
    UINT          cellY,        //テクスチャのコマ数縦(アニメーションしない場合は１を設定してください)

    UINT          md_frame,
    UINT          end_frame,

    DWORD         md_color,
    DWORD         end_color
    ):
    pParticle(pParticle),
    move(move),
    power(power),

    time(0),

    animation(animation),
    cellX(cellX),
    cellY(cellY),

    start_color(pParticle->m_Param.color.toDWORD()),
    md_color(md_color),
    end_color(end_color),

    md_frame(md_frame),
    end_frame(end_frame)
{
        //一度更新
        Update();

        //増えた分１フレーム戻す
        --time;
    }

ParticleMoveObject2::~ParticleMoveObject2()
{
    delete pParticle;
}


bool ParticleMoveObject2::Update()
{
    //移動
    pParticle->m_Param.pos += move;
    move += power;

    //色の補間
    {
        float t;
        if (time < md_frame)
        {
            t = (float)time / (float)md_frame;

            pParticle->m_Param.color = COLORf(ColorLerp(start_color, md_color, t));
        }
        else
        {
            t = (float)(time - md_frame) / (float)(end_frame - md_frame);

            pParticle->m_Param.color = COLORf(ColorLerp(md_color, end_color, t));
        }
        
    }
    //テクスチャアニメーション
    if (animation)
    {
        pParticle->SetCellUV((int)cellX, (int)cellY, (int)time);
    }

    //生存カウントorフィールドから離れすぎていると消える
    return
        end_frame > ++time &&
        pParticle->m_Param.pos.Length() < 150;
}


bool ParticleMoveObject2::Msg(MsgType mt)
{
    return false;
}


