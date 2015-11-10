#pragma once
#include "../GameSystem/ForwardDecl.h"
#include "../utillity/ColorUtility.h"

//---------------------------------------------//
// �@�@�@�G�t�F�N�g�o���֐��W
//---------------------------------------------//

namespace EffectFactory
{
    //���A�j���[�V�����G�t�F�N�g
    void Smoke(
        CrVector3 pos,
        CrVector3 move,
        float size,
        DWORD Color,
        bool Soft = false);

    //���G�t�F�N�g
    void SmokeParticle(
        CrVector3 pos,
        CrVector3 move,
        UINT   live_frame,
        float size,
        DWORD Color,
        bool Soft = false);

    //�J�E���^�[�̃A�j���[�V����
    void Counter(
        CrVector3 pos,
        float size);
	//�@�؂�ւ�
	void Change(CrVector3 pos,
		float size);

    //�~���傫���Ȃ�A�j���[�V����
    void CircleAnimationBillbord(
        CrVector3   pos,
        CrVector3   move,
        CrVector3   power,
        CrVector2   size,
        DWORD       color,
        DWORD       dw_flag
        );

    //�~���傫���Ȃ�A�j���[�V����
    void CircleAnimation(
        CrVector3   pos,
        CrVector3   vec,
        CrVector3   move,
        CrVector3   power,
        CrVector2   size,
        DWORD       color,
        DWORD       hdr_col
        );

    //�p�[�e�B�N��
	void Particle(
		UINT      type,
		UINT      live_time,
		CrVector3 pos,
		CrVector3 move,
		CrVector2 size,
		DWORD     Color,
		bool Soft = false,
		DWORD dw_Type = RS_COPY
        );

	//�p�[�e�B�N��
	void ParticleHDR(
		UINT      type,
		UINT      live_time,
		CrVector3 pos,
		CrVector3 move,
		CrVector2 size,
		DWORD     Color
		);


    //���S���̃G�t�F�N�g
    void DieEffect(
        CharacterBase* pDieCharacter,
        CrVector3      Damage_Vec
        );


    //�O�Ճp�[�e�B�N��
    void LocusParticle(
        CrVector3 pos,
        CrVector3 move,
        CrVector3 power,
        float     width,
        UINT      length,
        COLORf    color,
        COLORf    HDRcolor,
        UINT      live_frame,
        RATIO     bound = 0.2f
        );
}