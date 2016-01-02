#ifndef __SNAKESHOT_H__
#define __SNAKESHOT_H__

#include "../../Ball/Ball.h"
#include "../../GameSystem/GameObject.h"
#include "../../GameSystem/ForwardDecl.h"
#include "../../GameSystem/GameSystem.h"
#include "../../Damage/Damage.h"
#include "../../utillity/LocusHDR.h"

#include "SoccerPlayer.h"
#include "../CharacterBase.h"
#include "../../Effect/TornadoEffect.h"

//--------------------------------------------------------------------
//  サッカー必殺技のボールクラス
//--------------------------------------------------------------------

class Snakeshot :public BallBase,public GameObjectBase
{
public:
	//コンストラクタ
    Snakeshot(CrVector3 pos, CrVector3 vec, CharacterBase* pParent, RATIO power);
	~Snakeshot();

	bool Update();
	bool Msg(MsgType mt);

    CharacterBase* const m_pOriginParent;
    TornadoEffect*       m_pTornadoEffect;
	LpMeshRenderer		 m_pMeshRenderer;
	DamageShpere		 m_Damage;
	CharacterBase*       m_pTarget;
    RigidBody*           m_pRigidBody;
    Vector3              m_Angle;
    int                  m_Timer;

	void(Snakeshot::*m_pStatefunc)();

	void State_TargetDecision();
	void State_ToTagetMove();
	void State_NoHomingMove();
	void State_NoWork();
	void State_Delete();
    void State_Attack();

	bool isOutofField()const;
	void UpdateDamageClass();
	void UpdateMesh();
	bool isHitWall();
    void UpdateEffect();
	void FreezeGame(UINT frame);

	void Counter(CharacterBase* pCounterCharacter)override;

	void ToNoWork();

    void EffectApper(int n, RATIO scale);
public:
	//　遠距離ターゲット選定(もしなかったらnullptrを返す)
	CharacterBase* CalcTarget()const;
	//　ホーミング計算
    void MoveHomingRotate(Vector3 TargetPos);

};
#endif