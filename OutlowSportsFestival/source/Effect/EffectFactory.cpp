#include "EffectFactory.h"

#include "ParticleRenderer.h"
#include "ParticleHDRRenderer.h"

#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"

#include "AnimationBordRenderer.h"
#include "HitEffectObject.h"
#include "../character/CharacterBase.h"

#include "Effect/BlurImpact.h"

#include "../Camera/Camera.h"

void EffectFactory::Smoke(CrVector3 pos, CrVector3 move, float size, DWORD Color, bool Soft)
{
    ParticleRenderer* r = new ParticleRenderer();

    r->m_pTexture = DefResource.Get(Resource::TextureType::Anime_Smoke);
    r->m_Param.pos = pos;
    r->m_Param.color = Color;
    r->m_Param.dw_Flag = RS_COPY;
    r->m_Param.size = Vector2(size, size);
    r->m_SoftEnable = Soft;
    r->m_SoftDepth = 0.008f;

    ParticleMoveObject* m =
        new ParticleMoveObject(
        r,
        move,
        Vector3Zero,
        64,
        true,
        8,
        8
        );
}

//煙エフェクト
void EffectFactory::SmokeParticle(
    CrVector3 pos,
    CrVector3 move,
    UINT   live_frame,
    float size,
    DWORD Color,
    bool Soft)
{
    Particle(
        15,
        live_frame,
        pos,
        move,
        Vector2(size, size), 
        Color,
        Soft,
        RS_ADD
        );
}

void EffectFactory::Counter(CrVector3 pos, float size)
{
    ParticleHDRRenderer* r = new ParticleHDRRenderer();

    r->m_pTexture = DefResource.Get(Resource::TextureType::Anime_Counter);
    r->m_Param.pos = pos;
    r->m_Param.color = COLORf(0x40FFFFFF);
    r->m_HDRcolor = 0xA0FFFFFF;
    r->m_Param.dw_Flag = RS_COPY;
    r->m_Param.size = Vector2(size, size);
    r->m_Zenable = false;

    ParticleMoveObject* m =
        new ParticleMoveObject(
        r,
        Vector3Zero,
        Vector3Zero,
        32,
        true,
        8,
        4
        );

}

//　切り替え
void EffectFactory::Change(CrVector3 pos, float size)
{
	ParticleHDRRenderer* r = new ParticleHDRRenderer();

	r->m_pTexture = DefResource.Get(Resource::TextureType::Anime_Change);
	r->m_Param.pos = pos;
	r->m_Param.color = COLORf(0x40FFFFFF);
	r->m_HDRcolor = 0xA0FFFFFF;
	r->m_Param.dw_Flag = RS_COPY;
	r->m_Param.size = Vector2(size, size);
	r->m_Zenable = false;

	ParticleMoveObject* m =
		new ParticleMoveObject(
		r,
		Vector3Zero,
		Vector3Zero,
		32,
		true,
		8,
		4
		);

}

//円が大きくなるアニメーション
void EffectFactory::CircleAnimationBillbord(
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,
    DWORD       color,
    DWORD       dw_flag
    )
{
    //エフェクト
    ParticleRenderer* r = new ParticleRenderer();

    r->m_Param.color = COLORf(color);
    r->m_Param.dw_Flag = dw_flag;
    r->m_Param.pos = pos;
    r->m_Param.size = size;
    r->m_pTexture = DefResource.Get(Resource::TextureType::Anime_Circle);

    ParticleMoveObject* m = new ParticleMoveObject(
        r,
        move,
        power,
        16,
        true,
        4, 4
        );
}


//円が大きくなるアニメーション
void EffectFactory::CircleAnimation(
    CrVector3   pos,
    CrVector3   vec,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,
    DWORD       color,
    DWORD       hdr_col
    )
{
    Vector3 tvec(vec);

    AnimationBordRenderer* r = new AnimationBordRenderer(
        DefResource.Get(Resource::TextureType::Anime_Circle),
        4,
        4,
        16,
        color,
        hdr_col
        );

    r->m_Pos = pos;
    r->m_CellCount = 0;
    r->m_Size = size; 
    

    Vector3Cross(r->m_Right, tvec, Vector3AxisX);

    if (r->m_Right == Vector3Zero)
    {
        Vector3Cross(r->m_Right, tvec, Vector3AxisZ);
    }

    Vector3Cross(r->m_Up, r->m_Right, tvec);


    r->m_Right.Normalize();
    r->m_Up.Normalize();

    AnimationBordGameObj* m = new AnimationBordGameObj(
        r
        );

    m->animation_end_delete = true;
    m->animation_loop = false;
    m->animation_speed = 1.0f;

    m->move_power = power;
    m->move_speed = move;

}

//パーティクル
void EffectFactory::Particle(
    UINT      type,
    UINT      live_time,
    CrVector3 pos,
    CrVector3 move,
    CrVector2 size,
    DWORD     Color,
    bool Soft,
	DWORD dw_Type
    )
{
    ParticleRenderer* r = new ParticleRenderer();

    r->m_pTexture = DefResource.Get(Resource::TextureType::Particle);
    r->m_Param.pos = pos;
    r->m_Param.color = Color;
	r->m_Param.dw_Flag = dw_Type;
    r->m_Param.size = size;
    r->m_SoftEnable = Soft;
    r->m_SoftDepth = 0.008f;
    r->SetCellUV(4, 4, (int)type);

    ParticleMoveObject* m =
        new ParticleMoveObject(
        r,
        move,
        Vector3Zero,
        live_time,
        false,
        1,
        1
        );
}


//パーティクル
void EffectFactory::ParticleHDR(
	UINT      type,
	UINT      live_time,
	CrVector3 pos,
	CrVector3 move,
	CrVector2 size,
	DWORD     Color
	)
{
	ParticleHDRRenderer* r = new ParticleHDRRenderer();

	r->m_pTexture = DefResource.Get(Resource::TextureType::Particle);
	r->m_Param.pos = pos;
	r->m_Param.color = Color;
	r->m_HDRcolor = Color;
	r->m_Param.dw_Flag = RS_ADD;
	r->m_Param.size = size;
	r->SetCellUV(4, 4, (int)type);

	ParticleMoveObject* m =
		new ParticleMoveObject(
		r,
		move,
		Vector3Zero,
		live_time,
		false,
		1,
		1
		);
}


//死亡時のエフェクト
void EffectFactory::DieEffect(
    CharacterBase* pDieCharacter,
    CrVector3      Damage_Vec
    )
{
    COLORf EffectColor(CharacterBase::GetPlayerColor(pDieCharacter->m_PlayerInfo.number));

    //ヒットエフェクト作成
    new HitEffectObject(
        pDieCharacter->m_Params.pos + Vector3(0, 3, 0) + Damage_Vec*3.5f,
        Damage_Vec,
        0.05f,
        0.15f,
        Vector3(1,1,1)*4.0f,
        5,
        50
        );

    //ブラーエフェクト
    new BlurImpactSphere(
        pDieCharacter->m_Params.pos + Vector3(0, 3, 0),
        20,
        50,
        15
        );

    //カメラショック
    DefCamera.SetShock(
        Vector2(1, 1)*0.22f,
        20
        );
}