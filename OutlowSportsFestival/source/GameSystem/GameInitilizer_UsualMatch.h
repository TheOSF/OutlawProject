#pragma once
#include "GameInitializer.h"
#include "../GameScene/SceneGamePlay.h"

//------------------------------------------------------------
//  通常マッチゲーム初期化クラス
//------------------------------------------------------------

class GameInitializer_UsualMatch :public IGameInitializer
{
public:
    enum class StageType
    {
        Kasennziki,   //河川敷

    };

    GameInitializer_UsualMatch(StageType Stage, sceneGamePlay::InitParams& param);
    ~GameInitializer_UsualMatch();

    void GameCreate();

private:
    sceneGamePlay::InitParams m_Param;
    const StageType m_StageType;
};
