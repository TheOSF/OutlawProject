#pragma once

//--------------------------------------------------//
// ゲームのマッチかんりくらす　
//--------------------------------------------------//


/*
    試合の情報管理
    シーン移行管理
*/

class MatchManager
{
public:
    enum class NextSceneType
    {
        ReturnToTitle,
        ReturnToOption,

        GoResult,

        _ErrorType
    };

    static MatchManager& GetInstance();
    static void Release();

    void SceneChange(NextSceneType Next);

    bool GetNextScene(NextSceneType& Out);

private:

    static MatchManager* m_pInstance;

    NextSceneType m_NextSceneType;

    MatchManager();
    ~MatchManager();


};

#define DefMatchManager (MatchManager::GetInstance())