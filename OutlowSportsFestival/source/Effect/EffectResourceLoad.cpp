#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{


    //�q�b�g�G�t�F�N�g���b�V��
    DefResource.Regist(
        Resource::MeshType::HitEffect,
        new IEXMESH("DATA\\Effect\\HitEffect.imo")
        );



    //�~�A�j���[�V�����G�t�F�N�g
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );


}