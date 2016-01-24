#pragma once

#include "../BaseballPlayer.h"
#include "../../../GameSystem/ForwardDecl.h"
#include "../../../character/Computer/CharacterComputerMove.h"
#include "../../../character/Computer/CharacterComputerDoAction.h"
#include "../../../character/Computer/CharacterComputerReaction.h"

//****************************************************
//	野球プレイヤー(Com)の通常移動クラスヘッダー
//****************************************************
//コンピューターの移動クラス
class BaseballPlayerState_ComMove :public BaseballState
{
public:
	//　ステート開始
	void Enter(BaseballPlayer* b)override;
	//　ステート実行
	void Execute(BaseballPlayer* b)override;
	//　ステート終了
    void Exit(BaseballPlayer* b)override;

private:
	CharacterUsualMove*		    m_pMoveClass;
	CharacterComputerMove*      m_pMoveControllClass;
	CharacterComputerDoAction*  m_pDoActionClass;
	CharacterComputerReaction*  m_pReactionClass;
};

