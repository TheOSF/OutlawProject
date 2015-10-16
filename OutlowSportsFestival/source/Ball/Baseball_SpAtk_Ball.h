#ifndef __BASEBALL_SPATK_BALL_H__
#define __BASEBALL_SPATK_BALL_H__

#include "Ball.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../GameSystem/GameSystem.h"
#include "../Damage/Damage.h"
#include "../Render/LightObject.h"
#include "../Library/Bullet/BulletSystem.h"

//*****************************************************
//		通常玉クラス
//*****************************************************

class Baseball_SpAtk_Ball :public GameObjectBase, public BallBase
{
public:

	//物理パラメータ
	struct PhysicsParam
	{
		float Mass;
		float Friction;
		float Radius;
		float Restitution;
	};


	//コンストラクタ
	Baseball_SpAtk_Ball(
		BallBase::Params	params,			//ボールパラメータ
		DamageBase::Type	damage_type,	//ダメージ判定のタイプ
		float				damage_val,		//ダメージ量
		UINT                hit_num = 1   //ヒット数
		);
	~Baseball_SpAtk_Ball();

	//ボールのメッシュを返す
	static bool GetBallMesh(
		CharacterType::Value	type,	//ボールのキャラクタタイプ
		LPIEXMESH*				ppOut	//戻り値
		);

	//ボールのメッシュスケールを返す
	static float GetBallScale(
		CharacterType::Value	type    //ボールのキャラクタタイプ
		);

	//ボールの物理パラメータを返す
	static PhysicsParam GetBallPhysics(
		CharacterType::Value	type	//ボールのキャラクタタイプ
		);

	bool Update();
	bool Msg(MsgType mt);

private:

	bool(Baseball_SpAtk_Ball::*m_pStateFunc)();
	LpMeshRenderer		m_pMeshRenderer;
	DamageCapsure		m_Damage;
	int                 m_DeleteFrame;
	Matrix              m_BaseMatrix;
	RigidBody*          m_pRigitBody;
	const UINT          m_HitNum;
	UINT                m_HitCountSave;
	UINT                m_HitStopFrame;


	bool isOutofField()const;  //フィールド外に出ているか

	void UpdateDamageClass();  //ダメージ判定の位置を現在の位置に更新
	
	bool UpdateWallCheck(Vector3& outNewMove);    //壁との判定を取り、接触していたなら移動値を反射してステート移行をする
	
	void ToNoWork();           //攻撃判定のない状態にする

	bool StateFlyMove();
	bool StatePhysicMove();
};

#endif