#include "BaseballState_SPAttack_B.h"
#include "BaseballPlayerState.h"
#include "Computer\/BaseballPlayerState_ComMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/GlavityLocus.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/ThunderEffect.h"
#include "../../Camera/Camera.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"

//-------------�ߋ����U���X�e�[�g�N���X-------------

BaseballState_SPAttack_B::BaseballState_SPAttack_B(BaseballPlayer* b) :timeflg(false)
{

	m_Damage.m_Enable = false;
	m_Damage.HitCount = 0;
	m_Damage.m_Param.pos = Vector3Zero;
	m_Damage.m_Param.size = 3.0f;
	m_Damage.pBall = nullptr;
	m_Damage.pParent = b;
	m_Damage.type = DamageBase::Type::_VanishDamage;
	m_Damage.Value = 0.0f;
	m_Damage.m_Vec = Vector3AxisZ;

	m_Light.Visible = false;
}


BaseballState_SPAttack_B::~BaseballState_SPAttack_B()
{

}

// �X�e�[�g�J�n
void  BaseballState_SPAttack_B::Enter(BaseballPlayer* b)
{


	m_Timer = 0;

	m_pStateFunc = &BaseballState_SPAttack_B::State_Atk;

	m_pBaseBall = b;

	chr_func::XZMoveDown(b, 1);


	//�@�X�L���Q�[�W���Z�b�g
	//chr_func::ResetSkillGauge(b);

}


// �X�e�[�g���s
void BaseballState_SPAttack_B::Execute(BaseballPlayer* b)
{
	//�m�[�_���[�W
	DamageManager::HitEventBase NoDmgHitEvent;

	//�@���s
	(this->*m_pStateFunc)();

	//�@���G
	chr_func::UpdateAll(b, &NoDmgHitEvent);

	b->m_Renderer.Update(2);
	chr_func::CreateTransMatrix(b, b->m_ModelSize, &b->m_Renderer.m_TransMatrix);

	if (m_pStateFunc == &BaseballState_SPAttack_B::State_Finish)
	{

		if (b->m_PlayerInfo.player_type == PlayerType::_Player)
		{
			//�U���I�����ɒʏ�ړ����[�h�ɖ߂�
			b->SetState(new BaseballState_PlayerControll_Move());
		}
		else
		{
			b->SetState(new BaseballPlayerState_ComMove());
		}
	}
}

// �X�e�[�g�I��
void BaseballState_SPAttack_B::Exit(BaseballPlayer* b)
{

}



void BaseballState_SPAttack_B::State_Atk()
{
	m_Timer++;
	if (!timeflg)
	{
		//�@�������Đ�&The World
		if (m_Timer == 1)
		{
			Sound::Play(Sound::Skill);
			//�G�t�F�N�g
			new SpecialAttackEffect(m_pBaseBall, 55);
			FreezeGame(55);
		}
		//�@���G�t�F�N�g����
		ThunderInvoke(8);

		//�@���[�V�����J�n
		if (m_Timer == 1)
		{
			m_pBaseBall->m_Renderer.SetMotion(baseball_player::_mb_SpAtk_B);
		}

		/*if (m_Timer == 33)
		{
			m_pBaseBall->m_Renderer.SetMotion(baseball_player::_mb_SpAtk2);
		}*/
		if (m_Timer >= 56)
		{
			timeflg = true;
			m_Timer = 0;
		}
	}
	else
	{
		//�@���G�t�F�N�g����
		ThunderInvoke(8);

		//�@�����蔻��̏ꏊ�Ƃ�
		if (m_Timer == 1)
		{
			m_Damage.m_Enable = true;
			chr_func::GetFront(m_pBaseBall, &m_Damage.m_Param.pos);
			m_Damage.m_Vec = m_Damage.m_Param.pos;
			m_Damage.m_Vec.y = 0.8f;//�@��Ԋp�x
			m_Damage.m_VecPower.x = 1.0f;
			m_Damage.m_VecPower.y = 1.5f;
			m_Damage.m_Param.pos *= 3.0f;
			m_Damage.m_Param.pos += m_pBaseBall->m_Params.pos;
			m_Damage.Value = 50.0f;
			m_ChrLiveCount = DefCharacterMgr.GetCharacterLiveCount();
		}
		else
		{
			m_Damage.m_Enable = false;
		}

		if (m_Damage.HitCount > 0 && m_Timer == 2)
		{
			//�@�J�L�[��
			Sound::Play(Sound::BaseBall_SP);

			Vector3 power(0, -0.02f, 0);
			GlavityLocus* g;

			const Vector4
				stCol(1, 1, 1, 0.0f),
				endCol(1, 1, 1, 0);

			const Vector4
				stHdCol(1, 1, 1, 0.5f),
				endHdCol(1, 1, 1, 0);

			const Vector3 move = Vector3Normalize(m_Damage.m_Vec)*0.8f;

			Vector3 pos = m_Damage.m_Param.pos + Vector3(0, 3, 0);


			//�@�G�t�F�N�g
			for (int i = 0; i < 60; ++i)
			{
				g = new GlavityLocus(
					pos, move*1.5f + Vector3Rand() * 0.8f, power, 4, 40 + rand() % 30
					);

				g->m_BoundRatio = 0.2f;
				g->m_CheckWall = false;

				g->m_Locus.m_StartParam.Color = stCol;
				g->m_Locus.m_EndParam.Color = endCol;

				g->m_Locus.m_StartParam.HDRColor = stHdCol;
				g->m_Locus.m_EndParam.HDRColor = endHdCol;

				g->m_Locus.m_StartParam.Width = 0.1f;
				g->m_Locus.m_EndParam.Width = 0.00f;

			}

			//�@��
			for (int i = 0; i < 8; i++)
			{
				new ThunderEffect
					(pos/* - move*0.2f*i,*/,
					pos + move * 10 + Vector3Rand() * 10,
					4.5f,
					0.1f,
					10,
					Vector4(0, 0, 1, 0),
					15);
			}

			m_Light.param.color = Vector3(0, 0.4f, 0.8f);
			m_Light.param.pos = pos;
			m_Light.param.size = 50.0f;
			m_Light.Visible = true;


			//�@�u���[
			new BlurImpactSphere(
				pos,
				30,
				100,
				10
				);

			//�@��ʗh��
			DefCamera.SetShock(Vector2(0.5f, 0.5f), 50);

			if (m_ChrLiveCount == DefCharacterMgr.GetCharacterLiveCount())
			{
				std::list<LpGameObjectBase> UpdateObjList;

				DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 20);

			}
		}

		m_Light.param.size *= 0.9f;

		if (m_Timer == 30)
		{
			m_Timer = 0;
			m_pStateFunc = &BaseballState_SPAttack_B::State_Finish;

			if (m_pBaseBall->m_PlayerInfo.player_type == PlayerType::_Computer)
			{
				m_pBaseBall->setChangeFlg(true);
			}
		}
	}
}

//�@The World
void BaseballState_SPAttack_B::FreezeGame(UINT frame)
{
	std::list<LpGameObjectBase> UpdateObjList;

	UpdateObjList.push_back(m_pBaseBall);

	DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, frame, true);

	
}

//�@���G�t�F�N�g����
void BaseballState_SPAttack_B::ThunderInvoke(UINT point_num)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;
	Vector3 Thundderpos;
	m_pBaseBall->m_Renderer.GetWorldBoneMatirx(BoneMat, 8);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();
	Thundderpos = Pos + Forward*0.8f;

	new ThunderEffect
		(Pos,
		Thundderpos + Vector3Rand() * 3,
		1.5f,
		0.1f,
		17,
		Vector4(0, 0, 1, 0),
		point_num);
}