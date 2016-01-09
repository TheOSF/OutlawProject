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
//		�싅�����N���X
//*****************************************************

class BaseballPlayer;



class BaseballEquip :public GameObjectBase
{
//�@��{
public:
	enum class MeshType
	{
		Cap,
		Helmet,
		Grove,
		Bat
	};


	//�@�R���X�g���N�^
    BaseballEquip(CharacterRenderer* pRenderer, MeshType mt, float Scale);
	//�@�f�X�g���N�^
	~BaseballEquip();

	//�@����
	void Takeoff();

private:
    void UpdateMeshFromBonePos();
	//�@�����p�����[�^�Z�b�g
	void SetPhysicsParam();

    //�@����
    void Install(MeshType mt);

    bool Update();
    bool Msg(MsgType mt);

//�@�X�e�[�g(�����X�V���Ă����)
public:
	bool StateEquip();
	bool StatePhysicMove();
	bool StateEnd();


public:
	//�@��������
	void UpdatePhysicMove();
	
private:
    static const int DeleteTimeSet = 10;//�@���ł܂ł̎���

    CharacterRenderer* m_pChrRenderer;
    LPIEXMESH		m_pMesh;//�@��
    MeshType        m_MeshType;
    float           m_MeshScale;

    bool(BaseballEquip::*       m_pStateFunc)();
    LpMeshRenderer		        m_pMeshRenderer;
    LocusHDR                    m_Locus;
    RigidBody*                  m_pRigitBody;
    int                         m_DeleteFrame;
    int                         m_BoneNumber;

};