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
	//���[�����O�̕�������N���X
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

//�X�e�[�g�J�n
void BaseballPlayerState_ComMove::Enter(BaseballPlayer* b)
{

}


void BaseballPlayerState_ComMove::Execute(BaseballPlayer* b)
{


	//���f���̃��[���h�ϊ��s����X�V
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
////�@�ړ�
//void BaseballPlayerState_ComMove::doMove(BaseballPlayer* b)
//{
//	//�ړ��C�x���g�N���X
//	class BaseballMoveEvent :public CharacterUsualMove::MoveEvent
//	{
//		BaseballPlayer* m_pBaseball;
//	public:
//		BaseballMoveEvent(BaseballPlayer* pBaseball) :
//			m_pBaseball(pBaseball){}
//
//		//�A�j���[�V�����̍X�V
//		void Update(bool isRun, RATIO speed_ratio)
//		{
//			m_pBaseball->m_Renderer.Update(1);
//		}
//		//����n�߂Ƀ��[�V�������Z�b�g
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
//		//�����͂��߂Ƀ��[�V�������Z�b�g
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
//		//����I���
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
//	//�ړ��p�����[�^����
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
//    //�ړ��R���g���[���N���X�̍쐬
//    {
//        CharacterComputerMove::InParam param;
//
//        param.m_BestLenList.push_back({ 5, 0.5f });
//        param.m_BestLenList.push_back({ 20, 0.5f });
//
//        m_pMoveControllClass = new CharacterComputerMove(b, param);
//    }
//
//	//�ړ��N���X�̍쐬
//	m_pMoveClass = new CharacterUsualMove(
//		b,
//		p,
//		new BaseballMoveEvent(b),
//		new BaseballHitEvent(b)
//		);
//}
//
////�@�U��
//void BaseballPlayerState_ComMove::doAction(BaseballPlayer* b)
//{
//
//
//}


