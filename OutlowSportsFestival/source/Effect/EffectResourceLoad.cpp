#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{


    //ヒットエフェクトメッシュ
    DefResource.Regist(
        Resource::MeshType::HitEffect,
        new IEXMESH("DATA\\Effect\\HitEffect.imo")
        );



    //円アニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );


}