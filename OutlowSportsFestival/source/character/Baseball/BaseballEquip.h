#pragma once


#include "GameSystem/GameObject.h"
#include "GameSystem/ForwardDecl.h"
#include "GameSystem/GameSystem.h"
#include "Damage/Damage.h"
#include "utillity/LocusHDR.h"
#include "Render/LightObject.h"
#include "Library/Bullet/BulletSystem.h"

#include "BaseballPlayer.h"

//*****************************************************
//		野球装備クラス
//*****************************************************

#define DeleteTimeSet 10;//　消滅までの時間

class BaseballPlayer;

class BaseballEquip :public GameObjectBase
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

	struct Param
	{
		Matrix  BoneMat;
		Vector3 Forward;
		Vector3 TempPos;
		Vector3 Pos;
		Vector3 Move;
	};

	
public:
	BaseballEquip(BaseballPlayer* b);
	~BaseballEquip();
	void UpdateColor();//色を現在の親キャラクタの色に設定
	bool Update();
	bool Msg(MsgType mt);
	void ToPhysicMove();
private:
	BaseballPlayer* m_Baseball;
	LPIEXMESH		pHatMesh;
	LPIEXMESH	    pHelmetMesh;
	Param param;
	PhysicsParam pParam;
	bool tempflg;
	bool Deleteflg;//　消滅時間減少用
	bool(BaseballEquip::*       m_pStateFunc)();
	
	iexMesh* pMesh;

	LpMeshRenderer		        m_pHelmetMeshRenderer;
	LpMeshRenderer		        m_pHatMeshRenderer;
	Vector3                     m_RotateSpeed;
	const float         m_Glavity;
	LocusHDR                    m_Locus;
	
	Matrix					    m;
	Matrix              m_BaseMatrix;
	RigidBody*          m_pRigitBody;
	int                 m_DeleteFrame;
public:

	bool StateEquip();
	bool StateChangeEquip();

public:
	void EquipPos();
	void ChangeEquip();
	void UpdateMove();
	bool UpdateWallCheck();
	
	static bool GetEquipMesh(bool batter, LPIEXMESH* ppOut);
	void SetPhysicsParam();
	//void ToNoWork();

public:
	Matrix RendererGetMatrix();
	void RendererSetMatrix();
	void RendererHdr();
	void RenderDelete();

};