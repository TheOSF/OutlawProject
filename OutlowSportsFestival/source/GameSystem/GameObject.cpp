#include "GameObject.h"

GameObjectManager*	GameObjectManager::m_pInstance = nullptr;

GameObjectBase::GameObjectBase()
#ifdef _DEBUG
:m_ManagerDelete(false)
#endif
{
	MyAssert(DefGameObjMgr.Add(this), "�o�^���s");
}

GameObjectBase::~GameObjectBase()
{
#ifdef _DEBUG
    MyAssert(m_ManagerDelete, "�w�肳�ꂽ���@�ȊO��GameObjectBase��delete����܂���");
#endif
}

//-------------------------------------------------------------

GameObjectManager& GameObjectManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameObjectManager();
	}
	return *m_pInstance;
}

void GameObjectManager::Release()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void GameObjectManager::Update()
{
    if (m_FreezeFrame > 0)
    {
        --m_FreezeFrame;
        FreezeUpdate();
    }
    else
    {
        UsualUpdate();
    }
}

//�S�I�u�W�F�N�g�Ƀ��b�Z�[�W�𑗐M
UINT GameObjectManager::SendMsg(GameObjectBase::MsgType mt)
{
    UINT count = 0;

    for (auto& it : m_GameObjectMap)
    {
        if (it.first->Msg(mt))
        {
            ++count;
        }
    }
    return count;
}

//�����ȊO�̃I�u�W�F�N�g�̍X�V�������̃t���[���ԍs��Ȃ�
void GameObjectManager::FreezeOtherObjectUpdate(
    std::list<LpGameObjectBase> UpdateObjList,  //�X�V���s���I�u�W�F�N�g���X�g
    UINT OtherFreeze_frame                      //�t���[�Y������t���[��
    )
{
    m_FreezeUpdateList = UpdateObjList;
    m_FreezeFrame = OtherFreeze_frame;
}

bool GameObjectManager::Add(LpGameObjectBase pObj)
{
	if (m_GameObjectMap.find(pObj) != m_GameObjectMap.end())
	{
		return false;
	}

	m_GameObjectMap.insert(GameObjectMap::value_type(pObj, pObj));

	return true;
}

GameObjectManager::GameObjectManager():
m_FreezeFrame(0)
{

}

GameObjectManager::~GameObjectManager()
{
	for (auto it = m_GameObjectMap.begin();
		it != m_GameObjectMap.end();
		++it)
	{
#ifdef _DEBUG
        it->second->m_ManagerDelete = true;
#endif
		delete it->second;
	}
}


void GameObjectManager::FreezeUpdate()
{
    GameObjectMap::iterator it2;

    for (GameObjectList::iterator it = m_FreezeUpdateList.begin();
        it != m_FreezeUpdateList.end();
        ++it)
    {
        it2 = m_GameObjectMap.find(*it);

        MyAssert(it2 != m_GameObjectMap.end(), "GameObjectManager::FreezeUpdate�@�X�V����I�u�W�F�N�g��������܂���ł���");
    
        if ((*it)->Update() == false)
        {

#ifdef _DEBUG
            (*it)->m_ManagerDelete = true;
#endif

            delete *it;
            it = m_FreezeUpdateList.erase(it);

            continue;
        }
    }

}

void GameObjectManager::UsualUpdate()
{
    GameObjectMap::iterator it = m_GameObjectMap.begin();

    while (it != m_GameObjectMap.end())
    {
        if (it->second->Update() == false)
        {
#ifdef _DEBUG
            it->second->m_ManagerDelete = true;
#endif
            delete it->second;
            it = m_GameObjectMap.erase(it);

            continue;
        }
        ++it;
    }
}