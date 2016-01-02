#ifndef __USUAL_BALL_H__
#define __USUAL_BALL_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"
#include "BallEffect.h"

//*****************************************************
//		�ʏ�ʃN���X
//*****************************************************

class UsualBall :public GameObjectBase, public BallBase
{
public:

    //�����p�����[�^
    struct PhysicsParam
    {
        //float Height;
        float Mass;
        float Friction;
        float Radius;
        float Restitution;
    };

    //�X�L���Q�[�W�̐L�т��(AddSkillValueRatio*�_���[�W��)
    static const float AddSkillValueRatio;

	//�R���X�g���N�^
    UsualBall(
        BallBase::Params	params,			 //�{�[���p�����[�^
        DamageBase::Type	damage_type,	 //�_���[�W����̃^�C�v
        float				damage_val,		 //�_���[�W��
        UINT                hit_num = 1,     //�q�b�g��
        UINT                live_frame = 240 //��s����
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

    //�{�[���̕����p�����[�^��Ԃ�
    static PhysicsParam GetBallPhysics(
        CharacterType::Value	type	//�{�[���̃L�����N�^�^�C�v
        );

    // RigidBody�𐶐�����
    static RigidBody* CreateRigidBody(
        CharacterType::Value type, //�{�[���̃L�����N�^�^�C�v
        Params params // �{�[���̃p�����[�^
    );

	bool Update();
	bool Msg(MsgType mt);

    void SetRotateSpeed(CrVector3 AngleSpeed);

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();           //�U������̂Ȃ���Ԃɂ���

private:

    bool(UsualBall::*m_pStateFunc)();
	LpMeshRenderer		        m_pMeshRenderer;
	DamageCapsure		        m_Damage;
    int                         m_DeleteFrame;
    LocusHDR                    m_Locus;
    Matrix                      m_BaseMatrix;
    RigidBody*                  m_pRigitBody;
    const UINT                  m_HitNum;
    UINT                        m_HitCountSave;
    UINT                        m_HitStopFrame;
    int                         m_FlyLiveFrame;
    Vector3                     m_RotateSpeed;
    const CharacterType::Value  m_FirstParentType;
    BallEffect                  m_BallEffect;
    float                       m_MeshScale;


	bool isOutofField()const;  //�t�B�[���h�O�ɏo�Ă��邩

	void UpdateDamageClass();  //�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
    void UpdateColor();   //�F�����݂̐e�L�����N�^�̐F�ɐݒ�
    bool UpdateWallCheck(Vector3& outNewMove);    //�ǂƂ̔�������A�ڐG���Ă����Ȃ�ړ��l�𔽎˂��ăX�e�[�g�ڍs������
    void AddLocusPoint();      //�O�Ղ̃|�C���g�����݂̃p�����[�^�ň�_�ǉ�����
    void ScaleUpdate();


    RATIO GetMovePower()const;

    bool StateFlyMove();
    bool StatePhysicMove();
};

#endif