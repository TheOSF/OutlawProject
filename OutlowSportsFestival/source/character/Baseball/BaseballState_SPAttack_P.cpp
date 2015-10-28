#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/Baseball_SpAtk_Ball.h"

#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
#include "../../Sound/Sound.h"

//
//void  BaseballState_SPAttack_P::PlayerControllEvent::AngleControll(RADIAN angle)
//{
//	const CharacterBase* const pTargetCharacter = GetFrontTargetEnemy();
//
//	if (pTargetCharacter != nullptr)
//	{
//		//������]
//		chr_func::AngleControll(m_pBaseball, pTargetCharacter->m_Params.pos, angle);
//	}
//	else
//	{
//		const Vector2 Stick = controller::GetStickValue(controller::stick::left, m_pBaseball->m_PlayerInfo.number);
//
//		//�X�e�B�b�N�����ȏ�|����Ă��邩�ǂ���
//		if (Vector2Length(Stick) > 0.25f)
//		{
//			Vector3 Vec(Stick.x, 0, Stick.y);
//
//			//�X�e�B�b�N�l���J������Ԃ�
//			Vec = Vector3MulMatrix3x3(Vec, matView);
//
//			//�L�����N�^��]
//			chr_func::AngleControll(m_pBaseball, m_pBaseball->m_Params.pos + Vec, angle);
//		}
//	}
//}
//
//const CharacterBase*  BaseballState_SPAttack_P::PlayerControllEvent::GetFrontTargetEnemy()
//{
//	CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();
//
//	const float  AutoDistance = 10.0f;               //�������ł���ő勗��
//	const RADIAN AutoMaxAngle = D3DXToRadian(90);   //�������ł���ő�p�x
//
//	const CharacterBase* pTargetEnemy = nullptr;    //�^�[�Q�b�g�ێ��̃|�C���^
//	RADIAN MostMinAngle = PI;                       //�����Ƃ������p�x
//	RADIAN TempAngle;
//
//	Vector3 MyFront;      //���g�̑O���x�N�g��
//	chr_func::GetFront(m_pBaseball, &MyFront);
//
//	auto it = ChrMap.begin();
//
//	while (it != ChrMap.end())
//	{
//		//���g�����O
//		if (m_pBaseball->m_PlayerInfo.number == it->first->m_PlayerInfo.number ||
//			chr_func::isDie(it->first)
//			)
//		{
//			++it;
//			continue;
//		}
//
//		//���������ȏ�̃L�����N�^�����O����
//		if (Vector3Distance(it->first->m_Params.pos, m_pBaseball->m_Params.pos) > AutoDistance)
//		{
//			it = ChrMap.erase(it);
//			continue;
//		}
//
//		//�O�x�N�g���ƓG�ւ̃x�N�g���̊p�x���v�Z����
//		TempAngle = Vector3Radian(MyFront, (it->first->m_Params.pos - m_pBaseball->m_Params.pos));
//
//		//�p�x����ԋ���������X�V
//		if (TempAngle < MostMinAngle)
//		{
//			pTargetEnemy = it->first;
//			MostMinAngle = TempAngle;
//		}
//
//		++it;
//	}
//
//	return pTargetEnemy;
//
//}


//�@�R���X�g���N�^
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :
m_Timer(0),
timeflg(false),
m_pSpAttack_P(nullptr)
{

}


//�@�X�e�[�g�J�n
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	// ������(�o�b�^�[)�N���X�쐬
	m_pSpAttack_P = this->CreateSpAttack_P(b);
	m_pBaseBall = b;
	timeflg = false;
}


//�@�X�e�[�g���s
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b){
	
	if (!timeflg)
	{
		m_Timer++;
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			FreezeGame(20);
		}
		if (m_Timer == 21)
		{
			m_Timer = 0;
			timeflg = true;
		}
	}
	else
	{
		// �X�e�B�b�N�̒l�Z�b�g
		m_pSpAttack_P->SetStickValue(
			controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

		// �X�V
		if (m_pSpAttack_P->Update() == false)
		{
			return;
		}
	}
}

//�@�X�e�[�g�I��
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b){
	delete m_pSpAttack_P;
}


//�@�������N���X
CharacterShotAttack* BaseballState_SPAttack_P::CreateSpAttack_P(BaseballPlayer* b){
	class SpAttackEvent :public CharacterShotAttack::Event{
		BaseballPlayer* m_pBaseball;//�@�싅
	public:
		//�@�R���X�g���N�^
		SpAttackEvent(BaseballPlayer* pBaseball) :
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

		// �_���[�W����J�n & �{�[������
		void Shot()
		{
			BallBase::Params param;

			chr_func::GetFront(m_pBaseball, &param.move);
			param.move *= 10.0f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
			param.type = BallBase::Type::_BaseballSpecialAtk;

			new Baseball_SpAtk_Ball(param, DamageBase::Type::_VanishDamage, 1);
		}

		//�@�������U���J�n
		void AttackStart()override{
			//�@�����[�V����
			m_pBaseball->m_Renderer.SetMotion(baseball_player::_mb_Shot);
		}

		void AttackEnd()
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			m_pBaseball->SetState(new BaseballState_PlayerControll_Move());
		}
	};

	CharacterShotAttack::AttackParams atk;

	atk.AllFrame = 40;
	atk.AttackPower = 20;
	atk.MaxTurnRadian = PI / 4;
	atk.MoveDownSpeed = 1.2f;
	atk.ShotFrame = 15;

	return m_pSpAttack_P = new CharacterShotAttack(
		b,
		new SpAttackEvent(b),
		atk,
		new  BaseballHitEvent(b)
		);
}

void BaseballState_SPAttack_P::FreezeGame(UINT frame)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(m_pBaseBall);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame);
}

