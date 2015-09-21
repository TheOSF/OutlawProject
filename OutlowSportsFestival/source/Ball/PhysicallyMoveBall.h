#pragma once

//--------------------------------------------------------------------//
//   物理的な動き方をするボールクラス(一度あたると攻撃判定はなくなる)
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

    //物理パラメータ
    struct
    {
        float Mass;
        float Friction;
        float Restitution;
    }
    PhysicsParam;

    DamageShpere		m_Damage;
    bool                m_DeleteFlag;

    //コンストラクタ
    PhysicallyMoveBall(
        BallBase::Params	params,			//ボールパラメータ
        DamageBase::Type	damage_type,	//ダメージ判定のタイプ
        float				damage_val,		//ダメージ量
        float               glavity         //重力
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

