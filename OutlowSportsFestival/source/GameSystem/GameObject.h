#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <map>
#include <list>
#include "debug\DebugFunction.h"

class GameObjectManager;


//********************************************************************
//	�p������Ǝ����ōX�V����Q�[�����I�u�W�F�N�g�̃x�[�X�N���X
//********************************************************************

class GameObjectBase
{
public:
	enum MsgType
	{
		_PlayStart,	    //�v���C�J�n��(���E���h���Ƃ�)�ɑ����郁�b�Z�[�W
		_RoundReset,	//���E���h�I�����ɑ����郁�b�Z�[�W
        _TimeUp,        //�^�C���A�b�v���ɑ����郁�b�Z�[�W
        _WinPose,       //�����|�[�Y���s���ɌĂ΂�郁�b�Z�[�W
	};

	GameObjectBase();

	virtual bool Update() = 0;	//false��Ԃ��Ə�������
	virtual bool Msg(MsgType mt) = 0;
protected:
	friend class GameObjectManager;

    virtual ~GameObjectBase();

#ifdef _DEBUG
private:
    bool    m_ManagerDelete;
#endif

};

typedef GameObjectBase* LpGameObjectBase;


//********************************************************************
//	�I�u�W�F�N�g�N���X�̕`��E�X�V������}�l�[�W���N���X
//********************************************************************

class GameObjectManager
{
public:
	static GameObjectManager& GetInstance();
	static void Release();
	
	void Update();	//�S�I�u�W�F�N�g�X�V

    //�S�I�u�W�F�N�g�Ƀ��b�Z�[�W�𑗐M(�߂�l�F���b�Z�[�W��true��Ԃ����I�u�W�F�N�g�̐�)
	UINT SendMsg(
        GameObjectBase::MsgType mt  //���M���郁�b�Z�[�W
        );	
    
    //�����ȊO�̃I�u�W�F�N�g�̍X�V�������̃t���[���ԍs��Ȃ�
    void FreezeOtherObjectUpdate(
        std::list<LpGameObjectBase> UpdateObjList,  //�X�V���s���I�u�W�F�N�g���X�g
        UINT OtherFreeze_frame                      //�t���[�Y������t���[��
        );

private:

	typedef std::map<LpGameObjectBase, LpGameObjectBase> GameObjectMap;
    typedef std::list<LpGameObjectBase> GameObjectList;

	friend class GameObjectBase;

	static GameObjectManager*	m_pInstance;
	GameObjectMap				m_GameObjectMap;
    UINT                        m_FreezeFrame;
    GameObjectList              m_FreezeUpdateList;

	GameObjectManager();
	~GameObjectManager();

    void FreezeUpdate();
    void UsualUpdate();

	bool Add(LpGameObjectBase pObj);
	bool Erace(LpGameObjectBase pObj);
};

#define DefGameObjMgr (GameObjectManager::GetInstance())


#endif