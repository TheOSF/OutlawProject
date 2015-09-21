#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{

    //パーティクルテクスチャ
    DefResource.Regist(
        Resource::TextureType::Particle,
        new iex2DObj("DATA\\Texture\\particle.png")
        );

    //ヒットエフェクトメッシュ
    DefResource.Regist(
        Resource::MeshType::HitEffect,
        new IEXMESH("DATA\\Effect\\HitEffect.imo")
        );


    //軌跡テクスチャ
    DefResource.Regist(
        Resource::TextureType::Locus1,
        new iex2DObj("DATA\\Effect\\Locus1.png")
        );

    //軌跡テクスチャ2
    DefResource.Regist(
        Resource::TextureType::Locus2,
        new iex2DObj("DATA\\Effect\\Locus2.png")
        );


    //円アニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );

    //煙アニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Smoke,
        new iex2DObj("DATA\\Effect\\Smoke.png")
        );

    //カウンターアニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Counter,
        new iex2DObj("DATA\\Effect\\Counter2.png")
        );

    


}