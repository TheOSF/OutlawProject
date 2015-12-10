#include "SceneOption.h"
#include "../IexSystem/Framework.h"
#include "../GameScene/SceneGamePlay.h"

#include    "../GameSystem/GameObject.h"
#include "../GameSystem/GameInitilizer_UsualMatch.h"

SceneOption::SceneOption()
{

}

SceneOption::~SceneOption()
{
    DefGameObjMgr.Release(); //ゲームオブジェクト削除は一番初めに


}

void SceneOption::Update()
{

    if (true)
    {
        sceneGamePlay::InitParams param;

        GameInitializer_UsualMatch::StageType Stage = GameInitializer_UsualMatch::StageType::Kasennziki;
        const UINT Round = 3;
        const UINT Frame = 60 * 60 * 2;

        param.Round = Round;
        param.Time = Frame;
        param.PlayerArray.fill({ -1, PlayerType::_Player, CharacterType::_Tennis });

        //キャラ選択
		param.PlayerArray.at(0) = { 0, PlayerType::_Player, CharacterType::_Baseball };
        param.PlayerArray.at(1) = { 1, PlayerType::_Computer, CharacterType::_Baseball };
		param.PlayerArray.at(2) = { 2, PlayerType::_Computer, CharacterType::_Baseball };
        param.PlayerArray.at(3) = { 3, PlayerType::_Computer, CharacterType::_Baseball };
		
        param.pInitializer = new GameInitializer_UsualMatch(Stage, param);
        MainFrame->ChangeScene(new sceneGamePlay(param));
    }

}

void SceneOption::Render()
{

}