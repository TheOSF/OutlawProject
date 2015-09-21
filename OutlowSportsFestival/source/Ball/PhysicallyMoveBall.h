#pragma once

//--------------------------------------------------------------------//
//   �����I�ȓ�����������{�[���N���X(��x������ƍU������͂Ȃ��Ȃ�)
//--------------------------------------------------------------------//

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../utillity/Locus.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"


class PhysicallyMoveBall :public GameObjectBase, public BallBase
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

    DamageShpere		m_Damage;
    bool                m_DeleteFlag;

    //�R���X�g���N�^
    PhysicallyMoveBall(
        BallBase::Params	params,			//�{�[���p�����[�^
        DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
        float				damage_val,		//�_���[�W��
        float               glavity         //�d��
        );

    ~PhysicallyMoveBall();


    bool Update();
    bool Msg(MsgType mt);

private:

    LpMeshRenderer		m_pMeshRenderer;
    
    int                 m_DeleteFrame;
    D3DXQUATERNION		m_Ballrot;
    Locus               m_Locus;
    Matrix              m_BaseMatrix;
    RigidBody*          m_pRigitBody;
    const float         m_Glavity;

    bool isOutofField()const;
    void UpdateDamageClass();
    void UpdateMesh();
    void UpdateLocusColor();
    void UpdateMove();
    void SetHDR();
    void UpdateWallCheck();

    void Counter(CharacterBase* pCounterChr)override;

    void ToNoWork();
};

