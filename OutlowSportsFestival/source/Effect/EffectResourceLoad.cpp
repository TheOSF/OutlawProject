#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{

    //パーティクルテクスチャ
    DefResource.Regist(
        Resource::TextureType::White,
        new iex2DObj("DATA\\Texture\\White.png")
        );


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

    //木の葉テクスチャ
    DefResource.Regist(
        Resource::TextureType::Konoha,
        new iex2DObj("DATA\\Effect\\konoha.png")
        );
    
    //ハートテクスチャ
    DefResource.Regist(
        Resource::TextureType::Heart,
        new iex2DObj("DATA\\Effect\\hart_mini.png")
        );

    //円アニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );

    //煙アニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Smoke,
        new iex2DObj("DATA\\Effect\\SmokeReal.png")
        );

    //カウンターアニメーションエフェクト
    DefResource.Regist(
        Resource::TextureType::Anime_Counter,
        new iex2DObj("DATA\\Effect\\Counter2.png")
        );

	//カウンターアニメーションエフェクト
	DefResource.Regist(
		Resource::TextureType::Anime_Change,
		new iex2DObj("DATA\\Effect\\explode.png")
		);
    
    //サッカー必殺技、ヒットエフェクト
    DefResource.Regist(
        Resource::MeshType::Soccer_Special_Hit,
        new IEXMESH("DATA\\Effect\\SoccerGoal.imo")
        );

}