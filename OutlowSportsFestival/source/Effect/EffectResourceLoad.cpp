#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{

    //�~�A�j���[�V�����G�t�F�N�g
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );


}