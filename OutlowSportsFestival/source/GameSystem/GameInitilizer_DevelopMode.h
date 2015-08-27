#pragma once
#include "GameInitializer.h"


//------------------------------------------------------------
//  ゲーム初期化クラスインターフェース
//------------------------------------------------------------

class GameInitializer_DevelopMode :public IGameInitializer
{
public:
    GameInitializer_DevelopMode();
    ~GameInitializer_DevelopMode();

    void GameCreate();
};
