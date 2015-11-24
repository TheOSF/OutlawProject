#pragma once
#include "GameInitializer.h"
#include "../GameScene/SceneGamePlay.h"

//------------------------------------------------------------
//  �ʏ�}�b�`�Q�[���������N���X
//------------------------------------------------------------

class GameInitializer_UsualMatch :public IGameInitializer
{
public:
    enum class StageType
    {
        Kasennziki,   //�͐�~

    };

    GameInitializer_UsualMatch(StageType Stage, sceneGamePlay::InitParams& param);
    ~GameInitializer_UsualMatch();

    void GameCreate();

private:
    sceneGamePlay::InitParams m_Param;
    const StageType m_StageType;
};
