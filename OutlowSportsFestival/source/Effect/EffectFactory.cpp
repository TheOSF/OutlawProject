#include "EffectFactory.h"

#include "ParticleRenderer.h"
#include "ParticleHDRRenderer.h"

#include "ParticleMoveObject.h"
#include "../GameSystem/ResourceManager.h"

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


void EffectFactory::Counter(CrVector3 pos, float size)
{
    ParticleHDRRenderer* r = new ParticleHDRRenderer();

    r->m_pTexture = DefResource.Get(Resource::TextureType::Anime_Counter);
    r->m_Param.pos = pos;
    r->m_Param.color = COLORf(0x40FFFFFF);
    r->m_HDRcolor = 0x40FFFFFF;
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

//�~���傫���Ȃ�A�j���[�V����
void EffectFactory::CircleAnimation(
    CrVector3   pos,
    CrVector3   move,
    CrVector3   power,
    CrVector2   size,
    DWORD       color,
    DWORD       dw_flag
    )
{
    //�G�t�F�N�g
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


//�p�[�e�B�N��
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


//�p�[�e�B�N��
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