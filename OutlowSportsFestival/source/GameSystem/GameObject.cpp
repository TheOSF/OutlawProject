#include "GameObject.h"

GameObjectManager*	GameObjectManager::m_pInstance = nullptr;

GameObjectBase::GameObjectBase()
#ifdef _DEBUG
:m_ManagerDelete(false)
#endif
{
	MyAssert(DefGameObjMgr.Add(this), "登録失敗");
}

GameObjectBase::~GameObjectBase()
{
#ifdef _DEBUG
    MyAssert(m_ManagerDelete, "指定された方法以外でGameObjectBaseがdeleteされました");
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

//全オブジェクトにメッセージを送信
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

//引数以外のオブジェクトの更新を引数のフレーム間行わない
void GameObjectManager::FreezeOtherObjectUpdate(
    std::list<LpGameObjectBase> UpdateObjList,  //更新を行うオブジェクトリスト
    UINT OtherFreeze_frame                      //フリーズさせるフレーム
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

        MyAssert(it2 != m_GameObjectMap.end(), "GameObjectManager::FreezeUpdate　更新するオブジェクトが見つかりませんでした");
    
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