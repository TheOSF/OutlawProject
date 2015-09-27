#ifndef __USUAL_BALL_H__
#define __USUAL_BALL_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../utillity/Locus.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"

//*****************************************************
//		�ʏ�ʃN���X
//*****************************************************

class UsualBall :public GameObjectBase, public BallBase
{
public:

    //�����p�����[�^
    struct
    {
        float Mass;
        float Friction;
        float Restitution;
    }
    PhysicsParam;


	//�R���X�g���N�^
    UsualBall(
        BallBase::Params	params,			//�{�[���p�����[�^
        DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
        float				damage_val,		//�_���[�W��
        UINT                hit_num = 1   //�q�b�g��
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

	LpMeshRenderer		m_pMeshRenderer;
	DamageShpere		m_Damage;
    UINT                m_DeleteFrame;
	D3DXQUATERNION		m_Ballrot;
    Locus               m_Locus;
    Matrix              m_BaseMatrix;
    RigidBody*          m_pRigitBody;
    const UINT          m_HitNum;
    UINT                m_HitCountSave;
    UINT                m_HitStopFrame;

	bool isOutofField()const;
	void UpdateDamageClass();
	void UpdateMesh();
    void UpdateLocusColor();
    void UpdateMove();
    void SetHDR();
    void UpdateWallCheck();

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();
};

#endif