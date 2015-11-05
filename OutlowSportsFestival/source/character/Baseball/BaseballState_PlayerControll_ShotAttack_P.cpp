
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "BaseballState_PlayerControll_ShotAttack_P.h"

#include "Computer/BaseballPlayerState_ComMove.h"

#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"

#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Damage/Damage.h"
#include "../CharacterCounterClass.h"

#include "Sound/Sound.h"

//***************************************
//�@������(����)
//***************************************
class ShotAttackEvent_P;
//�@�R���X�g���N�^
BaseballState_PlayerControll_ShotAttack_P::BaseballState_PlayerControll_ShotAttack_P() :m_pShotAttackClass_P(nullptr){

}


//�@�X�e�[�g�J�n
void BaseballState_PlayerControll_ShotAttack_P::Enter(BaseballPlayer* b){
	// ������(�s�b�`���[)�N���X�쐬
	m_pShotAttackClass_P = this->CreateShotAttackClass_P(b);
}


//�@�X�e�[�g���s
void BaseballState_PlayerControll_ShotAttack_P::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pShotAttackClass_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	//�@Com�Ȃ�
	ComExcute(b);

	// �X�V
	if (m_pShotAttackClass_P->Update() == false)
	{
		return;
	}
}

//�@�X�e�[�g�I��
void BaseballState_PlayerControll_ShotAttack_P::Exit(BaseballPlayer* b){
	delete m_pShotAttackClass_P;
}


//�@�������N���X
CharacterShotAttack* BaseballState_PlayerControll_ShotAttack_P::CreateShotAttackClass_P(BaseballPlayer* b){
	class ShotAttackEvent_P :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
		MilderHoming* mild;
	public:
		//�@�{�[��
		BallBase::Params param;
		//�@�^�[�Q�b�g
		Vector3 target;
	public:
		//�@�R���X�g���N�^
		ShotAttackEvent_P(BaseballPlayer* pBaseball) :target(0,0,0),
			m_pBaseball(pBaseball){}
		//�@�X�V
		void Update()override{

			//�@���f���X�V
			m_pBaseball->m_Renderer.Update(1.0f);

			// �]���s��X�V
			chr_func::CreateTransMatrix(
				m_pBaseball,
				0.05f,
				&m_pBaseball->m_Renderer.m_TransMatrix);
		}
	public:
		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			//�@�������U��(param�v�Z)
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);

			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = UsualBall::UsualBallShotY;

			param.pParent = m_pBaseball;
			param.scale = 1.0f;
			param.type = BallBase::Type::_Milder;
			//����
			new MilderHoming(param,5,m_pBaseball);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot);
			//�@���ʉ�
			Sound::Play(Sound::Swing3);
		}

		void AttackEnd()

		{	//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			if (m_pBaseball->m_PlayerInfo.player_type == PlayerType::_Player)
			{
				//�@�v���C���[
				m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
			}
			else
			{
				//�@�R���s���[�^�[
				m_pBaseball->SetState(new BaseballPlayerState_ComMove());
			}
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 8;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 0.8f;
	atk.ShotFrame = 15;

	return m_pShotAttackClass_P = new CharacterShotAttack(
		b,
		new ShotAttackEvent_P(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

void BaseballState_PlayerControll_ShotAttack_P::ComExcute(BaseballPlayer* b)
{
	if (b->m_PlayerInfo.player_type == PlayerType::_Computer)
	{
		//�^�[�Q�b�g�I�聕�����␳

		CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

		const float  AutoDistance = 400.0f;               //�������ł���ő勗��
		const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x

		const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
		RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
		RADIAN TempAngle;

		Vector3 MyFront;      //���g�̑O���x�N�g��
		chr_func::GetFront(b, &MyFront);

		auto it = ChrMap.begin();

		while (it != ChrMap.end())
		{
			//���g�����O
			if (b->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
				chr_func::isDie(it->first)
				)
			{
				++it;
				continue;
			}

			//���������ȏ�̃L�����N�^�����O����
			if (Vector3Distance(it->first->m_Params.pos, b->m_Params.pos) > AutoDistance)
			{
				it = ChrMap.erase(it);
				continue;
			}

			//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
			TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - b->m_Params.pos));

			//�p�x����ԋ���������X�V
			if (TempAngle < MostMinAngle)
			{
				pTargetEnemy = it->first;
				MostMinAngle = TempAngle;
			}

			++it;
		}
	}

}