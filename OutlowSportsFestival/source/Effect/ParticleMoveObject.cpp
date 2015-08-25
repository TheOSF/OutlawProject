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
    pParticle->m_Param.color = COLORf::Lerp(start_color, COLORf(0, 0, 0, 0), (float)time / (float)delete_time);

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