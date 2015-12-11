#include "MatchManager.h"


MatchManager* MatchManager::m_pInstance = nullptr;

MatchManager& MatchManager::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new MatchManager();
    }

    return *m_pInstance;
}


void MatchManager::Release()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

void MatchManager::SceneChange(NextSceneType Next)
{
    m_NextSceneType = Next;
}

bool MatchManager::GetNextScene(NextSceneType& Out)
{
    Out = m_NextSceneType;
    return m_NextSceneType != NextSceneType::_ErrorType;
}

MatchManager::MatchManager():
m_NextSceneType(NextSceneType::_ErrorType)
{

}

MatchManager::~MatchManager()
{

}