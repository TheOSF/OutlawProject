#pragma once

//---------------------------------------------//
// �@�@�@�G�t�F�N�g�o���֐��W
//---------------------------------------------//

namespace EffectFactory
{
    //���A�j���[�V�����G�t�F�N�g
    void Smoke(CrVector3 pos, CrVector3 move, float size, DWORD Color,bool Soft = false);

    //�J�E���^�[�̃A�j���[�V����
    void Counter(CrVector3 pos, float size);

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
}