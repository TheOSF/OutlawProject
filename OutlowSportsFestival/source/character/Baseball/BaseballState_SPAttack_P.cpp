#include "BaseballState_SPAttack_P.h"
#include "BaseballPlayerState.h"
#include "Baseball_HitEvent.h"
#include "../../Ball/UsualBall.h"
#include "../../Ball/MilderHoming.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"
#include "../CharacterEvasionClass.h"
Baseball_SPAttack_P::Baseball_SPAttack_P(BaseballPlayer* b, BallBase::Params& param):
m_Locus(20),
m_DeleteFlag(false),
m_pBaseball(b)
{
	LPIEXMESH		pBallMesh;

	//�p�����[�^���
	m_Params = param;

	//�_���[�W����̃p�����[�^����
	m_Damage.pBall = this;
	m_Damage.pParent = param.pParent;
	m_Damage.m_Param.size = 1;	//�傫���̓{�[���ɂ���ĈقȂ�?
	m_Damage.type = DamageBase::Type::_WeekDamage;
	m_Damage.Value = 0.1f;
	m_Damage.m_Enable = true;
	UpdateDamageClass();

	//�{�[���̃��b�V�����쐬
	UsualBall::GetBallMesh(param.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//���b�V���̃����_���[�쐬(�ŏI�I�Ƀ��b�V�����g���܂킵�ĕ`�悷��ׂ�)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		false,
		MeshRenderer::RenderType::UseColor
		);

	D3DXQuaternionIdentity(&m_Ballrot);

	UpdateMesh();

	//�O�Ղ̐ݒ�
	m_Locus.m_Division = 0;
	m_Locus.m_StartParam.Width = 0.4f;
	m_Locus.m_EndParam.Width = 0.1f;

	UpdateLocusColor();
}

Baseball_SPAttack_P::~Baseball_SPAttack_P()
{
	delete m_pMeshRenderer;
}


bool Baseball_SPAttack_P::Update()
{
	UpdateMove();
	UpdateMesh();
	UpdateDamageClass();

	if (m_Params.type != BallBase::Type::_DontWork)
	{
		UpdateWallCheck();
	}

	Vector3 v;
	Vector3Cross(v, m_Params.move, DefCamera.GetForward());
	v.Normalize();

	m_Locus.AddPoint(m_Params.pos, v);

	return !m_DeleteFlag && !isOutofField() && m_DeleteFrame > 0;
}


bool Baseball_SPAttack_P::Msg(MsgType mt)
{

	return false;
}


bool Baseball_SPAttack_P::isOutofField()const
{
	return Vector3Length(m_Params.pos) > 100;
}


void Baseball_SPAttack_P::UpdateDamageClass()
{
	m_Damage.vec = m_Params.move;
	m_Damage.m_Param.pos = m_Params.pos;
}

void Baseball_SPAttack_P::UpdateMesh()
{
	//���b�V���̃��[���h�ϊ��s����X�V����

	Matrix m;
	const float s = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

	D3DXMatrixScaling(&m, s, s, s);	//�傫���̓{�[���ɂ���ĕς���K�v������

	m._41 = m_Params.pos.x;
	m._42 = m_Params.pos.y;
	m._43 = m_Params.pos.z;

	m_pMeshRenderer->SetMatrix(m);
}

void Baseball_SPAttack_P::UpdateLocusColor()
{
	const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

	m_Locus.m_StartParam.Color = Vector4(
		float((Color >> 16) & 0xFF) / 255.f,
		float((Color >> 8) & 0xFF) / 255.f,
		float(Color & 0xFF) / 255.f,
		0.5f
		);

	m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void Baseball_SPAttack_P::UpdateMove()
{

	m_Params.pos += m_Params.move;

	//�G�ɓ������Ă�����U��������Ȃ���
	if (m_Damage.HitCount > 0)
	{
		//�U������̂Ȃ���Ԃɂ���
		ToNoWork();
	}
}

void Baseball_SPAttack_P::UpdateWallCheck()
{
	Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
	float Dist = m_Params.move.Length()*2.0f;
	int Material;

	Vec.Normalize();

	if (DefCollisionMgr.RayPick(
		&Out,
		&Pos,
		&Vec,
		&Dist,
		&Material,
		CollisionManager::RayType::_Usual
		)
		)
	{
		//�U������̂Ȃ���Ԃɂ���
		ToNoWork();
	}

}

void Baseball_SPAttack_P::Counter(CharacterBase* pCounterCharacter)
{
	m_Damage.pParent = m_Params.pParent = pCounterCharacter;

	UpdateLocusColor();
}

void Baseball_SPAttack_P::ToNoWork()
{
	//�U������̂Ȃ���Ԃɂ���

	if (m_DeleteFlag)
	{
		return;
	}

	m_Params.type = BallBase::Type::_DontWork;
	m_Damage.m_Enable = false;
	m_DeleteFlag = true;

	m_BaseMatrix = m_pMeshRenderer->GetMatrix();

	m_BaseMatrix._41 = 0;
	m_BaseMatrix._42 = 0;
	m_BaseMatrix._43 = 0;
}

//�@�R���X�g���N�^
BaseballState_SPAttack_P::BaseballState_SPAttack_P() :m_pSpAttack_P(nullptr)
{

}


//�@�X�e�[�g�J�n
void BaseballState_SPAttack_P::Enter(BaseballPlayer* b)
{
	// ������(�o�b�^�[)�N���X�쐬
	m_pSpAttack_P = this->CreateSpAttack_P(b);
}


//�@�X�e�[�g���s
void BaseballState_SPAttack_P::Execute(BaseballPlayer* b){
	// �X�e�B�b�N�̒l�Z�b�g
	m_pSpAttack_P->SetStickValue(
		controller::GetStickValue(controller::stick::left, b->m_PlayerInfo.number));

	// �X�V
	if (m_pSpAttack_P->Update() == false)
	{
		return;
	}
}

//�@�X�e�[�g�I��
void BaseballState_SPAttack_P::Exit(BaseballPlayer* b){
	delete m_pSpAttack_P;
}


//�@�������N���X
CharacterShotAttack* BaseballState_SPAttack_P::CreateSpAttack_P(BaseballPlayer* b){
	class SpAttackEvent :public CharacterShotAttack::Event{
		MilderHoming* mild;
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
			param.move *= 0.5f;
			param.pos = m_pBaseball->m_Params.pos;
			param.pos.y = BallBase::UsualBallShotY;
			param.pParent = m_pBaseball;
			param.type = BallBase::Type::_Usual;

			new UsualBall(param, DamageBase::Type::_WeekDamage, 1);
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
	atk.AttackPower = 8;
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
