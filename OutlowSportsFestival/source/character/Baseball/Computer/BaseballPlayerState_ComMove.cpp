#include "BaseballPlayerState_ComMove.h"

#include "../Baseball_HitEvent.h"
#include "../BaseballPlayerState_Attack_B.h"
#include "../BaseballPlayerState_Attack_P.h"
#include "../BaseballState_PlayerControll_ShotAttack_B.h"
#include "../BaseballState_PlayerControll_ShotAttack_P.h"
#include "../BaseballState_SPAttack_B.h"
#include "../BaseballState_SPAttack_P.h"
#include "../BaseballState_Change.h"
#include "../BaseballPlayerState_Counter.h"
#include "../BaseballPlayerState.h"
#include "../BaseballState_PlayerControll_Evasion.h"
#include "../BaseballState_PoseMotion.h"

#include "../../../GameSystem/GameController.h"
#include "../../CharacterFunction.h"
#include "../../../Ball/UsualBall.h"
#include "../../../Damage/Damage.h"
#include "../../CharacterMoveClass.h"
#include "../../CharacterShotAttackClass.h"
#include "../../Computer/CharacterComputerReactionHitEvent.h"
#include "../../CharacterManager.h"


class BaseballPlayerComputerrUtillityClass
{
public:
	//ローリングの方向制御クラス
	class ComputerRollingControll :public  BaseballState_Rolling::CallBackClass
	{
	public:
		BaseballPlayer*const cb;
		ComputerRollingControll(BaseballPlayer* pb, Vector3 vec) :cb(cb), stick(vec) {}
		Vector3 stick;


		Vector3 GetVec()override
		{
			float v = PI / (1 + rand() % 4);
			Vector3 vec(stick.x + v, 0, stick.z - v);

			return vec;

		}
	};

};

//ステート開始
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
{

}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{


	//モデルのワールド変換行列を更新
	chr_func::CreateTransMatrix(b, &b->m_Renderer.m_TransMatrix);

}

void BaseballPlayerState_ComMove::Exit(BaseballPlayer* b)
{
	delete m_pMoveClass;
	delete m_pMoveControllClass;
	delete m_pDoActionClass;
	delete m_pReactionClass;
}
//
////　移動
//void BaseballPlayerState_ComMove::doMove(BaseballPlayer* b)
//{
//	//移動イベントクラス
//	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
//	{
//		BaseballPlayer* m_pBaseball;
//	public:
//		BaseballMoveEvent(BaseballPlayer* pBaseball) :
//			m_pBaseball(pBaseball){}
//
//		//アニメーションの更新
//		void Update(bool isRun, RATIO speed_ratio)
//		{
//			m_pBaseball->m_Renderer.Update(1);
//		}
//		//走り始めにモーションをセット
//		void RunStart()
//		{
//			if (m_pBaseball->getBatterFlg())
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run_B);
//			}
//			else
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Run_P);
//			}
//		}
//		//立ちはじめにモーションをセット
//		void StandStart()
//		{
//			if (m_pBaseball->getBatterFlg())
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_B);
//			}
//			else
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stand_P);
//			}
//		}
//
//		//走り終わり
//		void RunEnd()
//		{
//			if (m_pBaseball->getBatterFlg())
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stop_B);
//			}
//			else
//			{
//				m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Stop_P);
//			}
//		}
//	};
//
//	//移動パラメータを代入
//	CharacterUsualMove::Params p;
//
//
//	p.Acceleration = 0.15f;
//	p.MaxSpeed = 0.32f;
//	p.TurnSpeed = 0.3f;
//	p.DownSpeed = 0.08f;
//    p.RunEndFrame = 35;
//
//
//    //移動コントロールクラスの作成
//    {
//        CharacterComputerMove::InParam param;
//
//        param.m_BestLenList.push_back({ 5, 0.5f });
//        param.m_BestLenList.push_back({ 20, 0.5f });
//
//        m_pMoveControllClass = new CharacterComputerMove(b, param);
//    }
//
//	//移動クラスの作成
//	m_pMoveClass = new CharacterUsualMove(
//		b,
//		p,
//		new BaseballMoveEvent(b),
//		new BaseballHitEvent(b)
//		);
//}
//
////　攻撃
//void BaseballPlayerState_ComMove::doAction(BaseballPlayer* b)
//{
//
//
//}


