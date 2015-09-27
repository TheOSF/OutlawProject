
#include "ParticleManagerUpdater.h"
#include "ParticleManager.h"
#include "../GameSystem/ResourceManager.h"


ParticleManagerUpdater::ParticleManagerUpdater()
{

    {
        //パーティクルマネージャ初期化
        ParticleManager::Initialze(
        1000,
        DefResource.Get(Resource::TextureType::Particle),
        4,
        4
        );

    }
}

ParticleManagerUpdater::~ParticleManagerUpdater()
{
    ParticleManager::Release();
}

bool ParticleManagerUpdater::Update()
{
    DefParticle.Update();

    return true;
}

bool ParticleManagerUpdater::Msg(MsgType mt)
{
    return false;
}

void ParticleManagerUpdater::CalcZ()
{
    m_SortZ = 1;
}

void ParticleManagerUpdater::Render()
{
    DefParticle.Render();
}