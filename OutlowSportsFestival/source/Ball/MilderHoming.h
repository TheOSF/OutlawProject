#ifndef __MILDERHOMIG_H__
#define __MILDERHOMIG_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"

#include "../utillity/LocusHDR.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"

#include "../character/Baseball/BaseballPlayer.h"
#include "../utillity/Locus.h"

//*****************************************************
//		�ǔ��ʃN���X
//*****************************************************

class BallBase;

class MilderHoming :public BallBase, public GameObjectBase
{
private:
    float acc;//�@�����x
    int homingcounter;//�@�ǔ�����
	bool frontflg;//�@�O�����ɂ���悤
	BaseballPlayer* bp;
public:
    //�����p�����[�^
    struct
    {
        float Mass;
        float Friction;
        float Restitution;
    }
    PhysicsParam;
public:
    //�R���X�g���N�^
    MilderHoming(
        BallBase::Params	params,			//�{�[���p�����[�^
        float				damage_val,  	//�_���[�W��
		BaseballPlayer* b
        );
    ~MilderHoming();

    bool Update();
    bool Msg(MsgType mt);

    LpMeshRenderer		m_pMeshRenderer;
    DamageShpere		m_Damage;
    UINT                m_DeleteFrame;
    Matrix              m_BaseMatrix;
    RigidBody*          m_pRigitBody;
    Locus            m_Locus;

    BallBase			m_BallBase;
    CharacterBase*      m_pTarget;

    void(MilderHoming::*m_pStatefunc)();

    void State_TargetDecision();
    void State_ToTagetMove();
    void State_NoWork();
    void State_Delete();
	void State_Normal();


    bool isOutofField()const;
    void UpdateDamageClass();
    void UpdateMesh();
    void UpdateLocusColor();
    void SetHDR();
    bool isHitWall();

	void Cheak();

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();
public:
    //�@�������^�[�Q�b�g�I��(�����Ȃ�������nullptr��Ԃ�)
    CharacterBase* CalcTarget()const;
    //�@�z�[�~���O�v�Z
    void Homing(Vector3 TargetPos);

};
#endif