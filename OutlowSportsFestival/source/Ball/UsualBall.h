#ifndef __USUAL_BALL_H__
#define __USUAL_BALL_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../utillity/Locus.h"

//*****************************************************
//		�ʏ�ʃN���X
//*****************************************************

class UsualBall :public GameObjectBase
{
public:
	//�R���X�g���N�^
	UsualBall(
		BallBase::Params	params,			//�{�[���p�����[�^
		DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
		float				damage_val		//�_���[�W��
		);
	~UsualBall();

	//�{�[���̃��b�V����Ԃ�
	static bool GetBallMesh(
		CharacterType::Value	type,	//�{�[���̃L�����N�^�^�C�v
		LPIEXMESH*				ppOut	//�߂�l
		);

    //�{�[���̃��b�V���X�P�[����Ԃ�
    static float GetBallScale(
        CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
        );

	bool Update();
	bool Msg(MsgType mt);

private:
	BallBase			m_BallBase;
	LpMeshRenderer		m_pMeshRenderer;
	DamageShpere		m_Damage;
	const int			m_FreezeDeleteFrame;
	int					m_FreezeCount;
	D3DXQUATERNION		m_Ballrot;
    Locus               m_Locus;

	bool isOutofField()const;
	void UpdateDamageClass();
	void UpdateMesh();
    void UpdateLocusColor();
    void SetHDR();
};

#endif