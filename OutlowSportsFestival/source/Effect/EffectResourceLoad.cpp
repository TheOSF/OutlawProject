#include "EffectResourceLoad.h"
#include "../GameSystem/ResourceManager.h"


void EffectResource::Load()
{

    //�p�[�e�B�N���e�N�X�`��
    DefResource.Regist(
        Resource::TextureType::White,
        new iex2DObj("DATA\\Texture\\White.png")
        );


    //�p�[�e�B�N���e�N�X�`��
    DefResource.Regist(
        Resource::TextureType::Particle,
        new iex2DObj("DATA\\Texture\\particle.png")
        );

    //�q�b�g�G�t�F�N�g���b�V��
    DefResource.Regist(
        Resource::MeshType::HitEffect,
        new IEXMESH("DATA\\Effect\\HitEffect.imo")
        );


    //�O�Ճe�N�X�`��
    DefResource.Regist(
        Resource::TextureType::Locus1,
        new iex2DObj("DATA\\Effect\\Locus1.png")
        );

    //�O�Ճe�N�X�`��2
    DefResource.Regist(
        Resource::TextureType::Locus2,
        new iex2DObj("DATA\\Effect\\Locus2.png")
        );

    //�؂̗t�e�N�X�`��
    DefResource.Regist(
        Resource::TextureType::Konoha,
        new iex2DObj("DATA\\Effect\\konoha.png")
        );
    
    //�n�[�g�e�N�X�`��
    DefResource.Regist(
        Resource::TextureType::Heart,
        new iex2DObj("DATA\\Effect\\hart_mini.png")
        );

    //�~�A�j���[�V�����G�t�F�N�g
    DefResource.Regist(
        Resource::TextureType::Anime_Circle,
        new iex2DObj("DATA\\Effect\\Circle.png")
        );

    //���A�j���[�V�����G�t�F�N�g
    DefResource.Regist(
        Resource::TextureType::Anime_Smoke,
        new iex2DObj("DATA\\Effect\\SmokeReal.png")
        );

    //�J�E���^�[�A�j���[�V�����G�t�F�N�g
    DefResource.Regist(
        Resource::TextureType::Anime_Counter,
        new iex2DObj("DATA\\Effect\\Counter2.png")
        );

	//�J�E���^�[�A�j���[�V�����G�t�F�N�g
	DefResource.Regist(
		Resource::TextureType::Anime_Change,
		new iex2DObj("DATA\\Effect\\explode.png")
		);
    
    //�T�b�J�[�K�E�Z�A�q�b�g�G�t�F�N�g
    DefResource.Regist(
        Resource::MeshType::Soccer_Special_Hit,
        new IEXMESH("DATA\\Effect\\SoccerGoal.imo")
        );

}