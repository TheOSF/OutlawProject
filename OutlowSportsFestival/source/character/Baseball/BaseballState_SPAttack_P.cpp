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

		target = nullptr;
		target = CalcTarget();
		const float AngleSpeed = D3DXToRadian(3);

		//�^�[�Q�b�g��������
		if (target != nullptr)
		{
			chr_func::AngleControll(b, target->m_Params.pos, AngleSpeed*2.0f);
		}

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
			param.move *= 9.0f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
			param.type = BallBase::Type::_BaseballSpecialAtk;

			//�@�З͂Ƃ�
			new Baseball_SpAtk_Ball(param, DamageBase::Type::_VanishDamage, 50);
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


//�@�^�[�Q�b�g�I��
CharacterBase* BaseballState_SPAttack_P::CalcTarget()const
{
	Vector3 v1, v2;

	const float HomingAngle = PI / 8;
	float MostNear = 1000;
	float TempLen;
	CharacterBase* pTarget = nullptr;

	//�@map���
	const CharacterManager::CharacterMap& chr_map =
		DefCharacterMgr.GetCharacterMap();


	for (auto it = chr_map.begin(); it != chr_map.end(); ++it)
	{

		//�@����ł�or�����Ȃ�continue
		if (chr_func::isDie(it->first) ||
			it->first->m_PlayerInfo.number == m_pBaseBall->m_PlayerInfo.number)
		{
			continue;
		}

		//�@����p�v�Z
		chr_func::GetFront(m_pBaseBall, &v1);

		v2 = it->first->m_Params.pos - m_pBaseBall->m_Params.pos;
		v2.y = 0;

		//�p�x�O�Ȃ�K���Ă��Ȃ�
		if (Vector3Radian(v1, v2) > HomingAngle)
		{
			continue;
		}

		TempLen = v2.Length();

		if (MostNear > TempLen)
		{
			MostNear = TempLen;
			pTarget = it->first;
		}
	}

	return pTarget;
}
