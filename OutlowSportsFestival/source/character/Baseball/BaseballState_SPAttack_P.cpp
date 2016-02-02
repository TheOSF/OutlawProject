#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"
#include "Computer/BaseballPlayerState_ComMove.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"
//　コンストラクタ
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :
m_Timer(0)
{

}


//　ステート開始
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
    b->SetMotion(baseball_player::_mb_SpAtk_P);

    {
        std::list<LpGameObjectBase> UpdateObjList;

        UpdateObjList.push_back(b);

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 40, true);
    }

    
}


//　ステート実行
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b)
{
    const int ShotFrame = 40;
    const int EndFrame = 60;

    ++m_Timer;

    chr_func::XZMoveDown(b, 0.08f);

    if (m_Timer == ShotFrame)
    {
        Vector3 pos, move;

        chr_func::GetFront(b, &move);
        move *= 3.8f;
        pos = b->m_Params.pos;
        pos.y = BallBase::UsualBallShotY;

        //　威力とか
        new Baseball_SpAtk_Ball(b, pos, move, 25);
    }

	//　モーション更新とか
	b->ModelUpdate(1);
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);


    if (m_Timer == EndFrame)
    {
        b->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(b));
    }

}

//　ステート終了
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b)
{
    //　スキルゲージリセット
    chr_func::ResetSkillGauge(b);
}

