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
//		追尾玉クラス
//*****************************************************

class BallBase;

class MilderHoming :public BallBase, public GameObjectBase
{
private:
    float acc;//　加速度
    int homingcounter;//　追尾時間
	bool frontflg;//　前向きにするよう
	BaseballPlayer* m_Baseball;
public:
    //物理パラメータ
	//物理パラメータ
	struct PhysicsParam
	{
		float Mass;
		float Friction;
		float Radius;
		float Restitution;
	};
	//スキルゲージの伸びる量(AddSkillValueRatio*ダメージ量)
	static const float AddSkillValueRatio;
public:
    //コンストラクタ
    MilderHoming(
        BallBase::Params	params,			//ボールパラメータ
        float				damage_val,  	//ダメージ量
		BaseballPlayer* b
        );
    ~MilderHoming();

    bool Update();
    bool Msg(MsgType mt);
	//bool(UsualBal::*m_pStateFunc)();
    LpMeshRenderer		m_pMeshRenderer;
	DamageCapsure		m_Damage;
    UINT                m_DeleteFrame;
    Matrix              m_BaseMatrix;
    RigidBody*          m_pRigitBody;
    LocusHDR            m_Locus;
	int                         m_EffectFrameCount;
    BallBase			m_BallBase;
    CharacterBase*      m_pTarget;
	const CharacterType::Value  m_FirstParentType;

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
    //　遠距離ターゲット選定(もしなかったらnullptrを返す)
    CharacterBase* CalcTarget()const;
    //　ホーミング計算
    void Homing(Vector3 TargetPos);
public:
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
	bool StatePhysicMove();
	void AddLocusPoint();
};
#endif