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

class BaseballPlayer;



class BaseballEquip :public GameObjectBase
{
//　基本
public:
	enum class MeshType
	{
		Cap,
		Helmet,
		Grove,
		Bat
	};


	//　コンストラクタ
    BaseballEquip(CharacterRenderer* pRenderer, MeshType mt, float Scale);
	//　デストラクタ
	~BaseballEquip();

	//　解除
	void Takeoff();

private:
    void UpdateMeshFromBonePos();
	//　物理パラメータセット
	void SetPhysicsParam();

    //　装着
    void Install(MeshType mt);

    bool Update();
    bool Msg(MsgType mt);

//　ステート(自動更新してくれる)
public:
	bool StateEquip();
	bool StatePhysicMove();
	bool StateEnd();


public:
	//　物理挙動
	void UpdatePhysicMove();
	
private:
    static const int DeleteTimeSet = 10;//　消滅までの時間

    CharacterRenderer* m_pChrRenderer;
    LPIEXMESH		m_pMesh;//　頭
    MeshType        m_MeshType;
    float           m_MeshScale;

    bool(BaseballEquip::*       m_pStateFunc)();
    LpMeshRenderer		        m_pMeshRenderer;
    LocusHDR                    m_Locus;
    RigidBody*                  m_pRigitBody;
    int                         m_DeleteFrame;
    int                         m_BoneNumber;

};