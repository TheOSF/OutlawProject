//#pragma once
//
//
//#include "GameSystem/GameObject.h"
//#include "GameSystem/ForwardDecl.h"
//#include "GameSystem/GameSystem.h"
//#include "Damage/Damage.h"
//#include "utillity/LocusHDR.h"
//#include "Render/LightObject.h"
//#include "Library/Bullet/BulletSystem.h"
//
//#include "BaseballPlayer.h"
//
////*****************************************************
////		野球装備クラス
////*****************************************************
//
//#define DeleteTimeSet 10;//　消滅までの時間
//
//class BaseballPlayer;
//
//
//
//class BaseballEquip :public GameObjectBase
//{
//public:
//	//物理パラメータ
//	struct PhysicsParam
//	{
//		float Mass;
//		float Friction;
//		float Radius;
//		float Restitution;
//	};
//
//	struct Param
//	{
//		Matrix  BoneMat;
//		Vector3 Forward;
//		Vector3 TempPos;
//		Vector3 Pos;
//		Vector3 Move;
//	};
//
//
//private:
//	BaseballPlayer* m_Baseball;
//	LPIEXMESH		pHedMesh;
//	//LPIEXMESH	    pHelmetMesh;
//	Param param;
//	PhysicsParam pParam;
//	bool tempflg;
//	bool Deleteflg;//　消滅時間減少用
//	bool(BaseballEquip::*       m_pStateFunc)();
//
//	iexMesh* pMesh;
//
//	LpMeshRenderer		        m_pHedMeshRenderer;
//	LpMeshRenderer		        m_pHatMeshRenderer;
//	Vector3                     m_RotateSpeed;
//	const float         m_Glavity;
//	LocusHDR                    m_Locus;
//
//	Matrix					    m;
//	Matrix              m_BaseMatrix;
//	RigidBody*          m_pRigitBody;
//	int                 m_DeleteFrame;
//
////　基本
//public:
//	enum MeshType
//	{
//		Cap,
//		Helmet,
//		Grove,
//		Bat
//	};
//	MeshType meshType;
//	//　コンストラクタ
//	BaseballEquip(BaseballPlayer* b, int BoneNum, MeshType mt);
//	//　デストラクタ
//	~BaseballEquip();
//	//　装着
//	void Install(MeshType mt);
//	//　解除
//	void Takeoff();
//	//　通常移動
//	void NormalMove_Hed();
//	//　バウンド&フェードアウト
//	void Bound();
//	// 後方に落ちる
//	void ToPhysicMove();
//
//	bool Update();
//	bool Msg(MsgType mt);
//
////　基本内で使う
//public:
//
//
//	void SetPhysicsParam();
////　ステート(自動更新してくれる)
//public:
//	bool StateEquip();
//	bool StateChangeEquip();
//	bool StateEnd();
//
//
//public:
//
//	void UpdateColor();//色を現在の親キャラクタの色に設定
//	void UpdateMove();
//	bool UpdateWallCheck();
//	
//public:
//	Matrix RendererGetMatrix();
//	void RendererSetMatrix();
//	void RendererHdr();
//	void RenderDelete();
//
//};