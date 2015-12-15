//*****************************************************************************************************************************
//
//		メインシーン
//
//*****************************************************************************************************************************
#pragma once
#include "../IexSystem/Scene.h"
#include "../GameSystem/GameInitializer.h"
#include "../GameSystem/GameSystem.h"
#include <array>

class	sceneGamePlay : public Scene
{
public:
    struct InitParams
    {
        struct PlayerInfo
        {
            int number;
            PlayerType::Value player_type;
            CharacterType::Value chr_type;
            StrongType::Value strong_type;
        };

        IGameInitializer*         pInitializer;
        std::array<PlayerInfo, 4> PlayerArray;
        UINT  Round;
        UINT  Time;
    };

    sceneGamePlay(
        InitParams& params
        );

	~sceneGamePlay();
	//	初期化
	bool Initialize();
	//	更新・描画
	void Update();	//	更新
	void Render();	//	描画

private:
    InitParams  m_Params;

    void UpdateSceneChange();
    void GoResult();
};

