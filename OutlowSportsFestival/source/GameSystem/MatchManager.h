#pragma once

//--------------------------------------------------//
// �Q�[���̃}�b�`����肭�炷�@
//--------------------------------------------------//


/*
    �����̏��Ǘ�
    �V�[���ڍs�Ǘ�
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