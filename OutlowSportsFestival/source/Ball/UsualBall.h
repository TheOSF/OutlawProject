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
//		通常玉クラス
//*****************************************************

class UsualBall :public GameObjectBase, public BallBase
{
public:

    //物理パラメータ
    struct PhysicsParam
    {
        //float Height;
        float Mass;
        float Friction;
        float Radius;
        float Restitution;
    };

    //スキルゲージの伸びる量(AddSkillValueRatio*ダメージ量)
    static const float AddSkillValueRatio;

	//コンストラクタ
    UsualBall(
        BallBase::Params	params,			 //ボールパラメータ
        DamageBase::Type	damage_type,	 //ダメージ判定のタイプ
        float				damage_val,		 //ダメージ量
        UINT                hit_num = 1,     //ヒット数
        UINT                live_frame = 240 //飛行時間
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

    //ボールの物理パラメータを返す
    static PhysicsParam GetBallPhysics(
        CharacterType::Value	type	//ボールのキャラクタタイプ
        );

    // RigidBodyを生成する
    static RigidBody* CreateRigidBody(
        CharacterType::Value type, //ボールのキャラクタタイプ
        Params params // ボールのパラメータ
    );

	bool Update();
	bool Msg(MsgType mt);

    void SetRotateSpeed(CrVector3 AngleSpeed);

    void Counter(CharacterBase* pCounterCharacter)override;

    void ToNoWork();           //攻撃判定のない状態にする

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


	bool isOutofField()const;  //フィールド外に出ているか

	void UpdateDamageClass();  //ダメージ判定の位置を現在の位置に更新
    void UpdateColor();   //色を現在の親キャラクタの色に設定
    bool UpdateWallCheck(Vector3& outNewMove);    //壁との判定を取り、接触していたなら移動値を反射してステート移行をする
    void AddLocusPoint();      //軌跡のポイントを現在のパラメータで一点追加する
    void ScaleUpdate();


    RATIO GetMovePower()const;

    bool StateFlyMove();
    bool StatePhysicMove();
};

#endif