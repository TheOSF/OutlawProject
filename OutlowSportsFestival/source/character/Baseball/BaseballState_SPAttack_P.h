#pragma once

#include "BaseballPlayer.h"
#include "../../Ball/Ball.h"
#include "../../Damage/Damage.h"
#include "../../utillity/Locus.h"
#include "../../Render/MeshRenderer.h"

//***************************************************
//		野球(ピッチャー)必殺技
//***************************************************

class Baseball_SPAttack_P :public GameObjectBase, public BallBase
{
public:

	//コンストラクタ
	Baseball_SPAttack_P(BaseballPlayer* b, BallBase::Params& param);
	~Baseball_SPAttack_P();

	bool Update();
	bool Msg(MsgType mt);

private:
	BaseballPlayer* const m_pBaseball;
	LpMeshRenderer		m_pMeshRenderer;
	DamageShpere		m_Damage;
	UINT                m_DeleteFrame;
	D3DXQUATERNION		m_Ballrot;
	Locus               m_Locus;
	Matrix              m_BaseMatrix;
	bool                m_DeleteFlag;

	bool isOutofField()const;

	void UpdateDamageClass();
	void UpdateMesh();
	void UpdateLocusColor();
	void UpdateMove();
	void UpdateWallCheck();
	void ToNoWork();

	void Counter(CharacterBase* pCounterCharacter)override;
};

//***************************************************
//		野球(ピッチャー)必殺技ステートクラス
//***************************************************
class CharacterShotAttack;
class BaseballState_SPAttack_P : public BaseballState
{
private:
	CharacterShotAttack* CreateSpAttack_P(BaseballPlayer* b);
	CharacterShotAttack* m_pSpAttack_P;
public:

	BaseballState_SPAttack_P();
	

	// ステート開始
	void Enter(BaseballPlayer* b)override;

	// ステート実行
	void Execute(BaseballPlayer* b)override;

	// ステート終了
	void Exit(BaseballPlayer* b)override;


};

