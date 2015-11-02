#include "GameObject.h"
#include <typeinfo>
#include <time.h>
#include <chrono>

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
    if (isFreezeFrame())
    {
        --m_FreezeFrame;
        FreezeUpdate();
    }
    else
    {
        UsualUpdate();
    }

    //デバッグ用

    if (KEY(KEY_ENTER, 0) == 3)
    {
        MyDebugString("\n-----現在のオブジェクト-------\n\n");
        
        for (auto& it : m_GameObjectMap)
        {
            const char* p = typeid(*it.first).name();
            MyDebugString("%s \n", p);
        }

        MyDebugString("\n-----総数 = %d -------\n\n", (int)m_GameObjectMap.size());
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
    UINT OtherFreeze_frame,                      //フリーズさせるフレーム
    bool FreezeAddGameObject            //フリーズ中に追加したゲームオブジェクトをフリーズさせるかどうか
    )
{
    m_FreezeUpdateList = UpdateObjList;
    m_FreezeFrame = OtherFreeze_frame;
    m_FreezeAddGameObject = FreezeAddGameObject;
}

bool GameObjectManager::Add(LpGameObjectBase pObj)
{
	if (m_GameObjectMap.find(pObj) != m_GameObjectMap.end())
	{
		return false;
	}

	m_GameObjectMap.insert(GameObjectMap::value_type(pObj, pObj));

    //フリーズ中の更新オブジェクトの追加
    if (isFreezeFrame() && m_FreezeAddGameObject)
    {
        m_FreezeUpdateList.push_back(pObj);
    }

	return true;
}

bool GameObjectManager::isFreezeFrame()const
{
    return m_FreezeFrame > 0;
}

GameObjectManager::GameObjectManager():
m_FreezeFrame(0),
m_FreezeAddGameObject(false)
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
    GameObjectList::iterator it = m_FreezeUpdateList.begin();

    while (
        it != m_FreezeUpdateList.end()
        )
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
            m_GameObjectMap.erase(it2);

            continue;
        }

        ++it;
    }

}

//#define _UPDATE_SPEED_CHECK

#ifdef _UPDATE_SPEED_CHECK



void GameObjectManager::UsualUpdate()
{
    GameObjectMap::iterator it = m_GameObjectMap.begin();

    auto All_time = std::chrono::system_clock::now();

    MyDebugString("\n\n---------------GameObjectの更新開始---------------- \n\n");

    while (it != m_GameObjectMap.end())
    {
        auto Func_time = std::chrono::system_clock::now();

        if (it->second->Update() == false)
        {
#ifdef _DEBUG
            it->second->m_ManagerDelete = true;
#endif
            delete it->second;
            it = m_GameObjectMap.erase(it);

            continue;
        }

        MyDebugString("--%s = %d  \n", typeid(*it->first).name(), 
            (int)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - Func_time).count()));

        ++it;
    }

    MyDebugString("--\n-- 総更新更新時間 %d  ", (int)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - All_time).count()));
    MyDebugString("\n--\n---------------END---------------- \n\n");
}

#else

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

#endif