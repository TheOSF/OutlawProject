#pragma once
#include "../CharacterBase.h"
#include "CharacterComputerMove.h"

class CharacterBase;

//--------------------------------------------------//
//  CharacterComputerDoActionクラスは内部クラスである
//  ActionEventクラスのAttack関数を
//　コンピュータが攻撃するべき時に呼ぶクラスである
//--------------------------------------------------//

class CharacterComputerDoAction
{
public:

	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
        virtual bool Action(CharacterBase* pTarget, float len) = 0;			//攻撃開始時に呼ばれる関数
	};

	CharacterComputerDoAction(
		CharacterBase*			pParent,	            //操るキャラクタのポインタ
        CharacterComputerMove*  pCharacterComputerMove, //AI移動クラスへのポインタ
		ActionEvent*			pActionEvent	        //移動イベントに反応するクラス
		);

	~CharacterComputerDoAction();

	void Update();

private:
    CharacterComputerMove* const    m_pCharacterComputerMove;
    CharacterBase* const            m_pChr;
	ActionEvent* const              m_pActionEvent;


    bool                 m_DoAttack;

    bool isDoAttack();
};