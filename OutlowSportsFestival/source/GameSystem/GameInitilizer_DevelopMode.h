#pragma once
#include "GameInitializer.h"


//------------------------------------------------------------
//  �Q�[���������N���X�C���^�[�t�F�[�X
//------------------------------------------------------------

class GameInitializer_DevelopMode :public IGameInitializer
{
public:
    GameInitializer_DevelopMode();
    ~GameInitializer_DevelopMode();

    void GameCreate();
};
