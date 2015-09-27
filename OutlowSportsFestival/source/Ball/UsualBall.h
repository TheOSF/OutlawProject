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
//		通常玉クラス
//*****************************************************

class UsualBall :public GameObjectBase, public BallBase
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


	//コンストラクタ
    UsualBall(
        BallBase::Params	params,			//ボールパラメータ
        DamageBase::Type	damage_type,	//ダメージ判定のタイプ
        float				damage_val,		//ダメージ量
        UINT                hit_num = 1   //ヒット数
		);
	~UsualBall();

	//ボールのメッシュを返す
	static bool GetBallMesh(
		CharacterType::Value	type,	//ボールのキャラクタタイプ
		LPIEXMESH*				ppOut	//戻り値
		);

    //ボールのメッシュスケールを返す
    static float GetBallScale(
        CharacterType::Value	type    //ボールのキャラクタタイプ
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