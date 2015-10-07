#ifndef __SNAKESHOT_H__
#define __SNAKESHOT_H__

#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"

#include "SoccerPlayer.h"

class BallBase;

class Snakeshot :public BallBase,public GameObjectBase
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
public:
	//コンストラクタ
	Snakeshot(
		BallBase::Params	params,			//ボールパラメータ
		float				damage_val   	//ダメージ量
		);
	~Snakeshot();

	bool Update();
	bool Msg(MsgType mt);

	LpMeshRenderer		m_pMeshRenderer;
	DamageShpere		m_Damage;
	UINT                m_DeleteFrame;
	Matrix              m_BaseMatrix;
	RigidBody*          m_pRigitBody;
	LocusHDR            m_Locus;

	BallBase			m_BallBase;
	CharacterBase*      m_pTarget;

	void(Snakeshot::*m_pStatefunc)();

	void State_TargetDecision();
	void State_ToTagetMove();
	void State_NoWork();
	void State_Delete();


	bool isOutofField()const;
	void UpdateDamageClass();
	void UpdateMesh();
	void UpdateLocusColor();
	void SetHDR();
	bool isHitWall();


	void Counter(CharacterBase* pCounterCharacter)override;

	void ToNoWork();
public:
	//　遠距離ターゲット選定(もしなかったらnullptrを返す)
	CharacterBase* CalcTarget()const;
	//　ホーミング計算
	void Homing(Vector3 TargetPos);

};
#endif