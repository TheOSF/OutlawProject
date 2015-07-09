#include "BaseballPlayer.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		�싅�v���C���[�N���X
//*************************************************************

//�@�R���X�g���N�^
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(true), acc(0.2f), outcounter(0), characounter(0),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);
}

//�@�f�X�g���N�^
BaseballPlayer::~BaseballPlayer(){
	delete m_pStateMachine;
}

//�@�X�e�[�g�Z�b�g
void BaseballPlayer::SetState(BaseballState* state){
	//�@�V�����X�e�[�g���Z�b�g
	m_pStateMachine->set_state(state);
}

//�@�X�V
bool BaseballPlayer::Update(){
	//�@�؂�ւ�
	Change();
	// �X�e�[�g���s
	m_pStateMachine->state_execute();

	return true;	//���true��Ԃ��Ə�������Ȃ�
}

bool  BaseballPlayer::Msg(MsgType mt){
	return m_pStateMachine->Msg(mt);
}

//�@�؂�ւ�
void BaseballPlayer::Change(){
	if (controller::GetTRG(controller::button::_L1, m_PlayerInfo.number)){
		if (batterflg){
			batterflg = false;
		}
		else{
			batterflg = true;
		}
	}

}

//�@�������֐���
BallBase::Params BaseballPlayer::BaseballShot(BaseballPlayer* b, BallBase::Params p){
	//�ړ��͑O����
	chr_func::GetFront(b, &p.move);
	//�X�s�[�h�͓K��
	p.move *= 0.45f;
	//�L�����̏ꏊ��(�ŏI�I�ɘr�̈ʒu�ɁH)
	p.pos = b->m_Params.pos;
	//�������L�������ʃ{�[�����˂�Y��
	p.pos.y = BallBase::UsualBallShotY;
	//�e��������
	p.pParent = b;
	//�ʏ�^�C�v
	p.type = BallBase::Type::_Usual;

	return p;
}

//�@�������^�[�Q�b�g�I��
BallBase::Params BaseballPlayer::TargetDecision(BallBase::Params p, Vector3 t){
	//�@����p�p
	Vector3 vf(0, 0, 0);
	Vector3 vt(0, 0, 0);
	float cross = 0;
	float dot = 0;

	//�@map���
	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();

	//�@�����֌W
	Vector3 v(0, 0, 0);
	float len = 2000.0f;
	float templen = 0.0f;

	//�@pos�ꎞ�ۑ��p
	Vector3 temppos(0, 0, 0);

	//�@�z�[�~���O�֌W
	D3DXQUATERNION q, invq, qpos;
	Vector3 v1, v2, axis;


	//�L�����N�^���W���Z�o
	for (auto it = chr_map.begin(); it != chr_map.end(); ++it){
		characounter++;
		//�@����ł���continue
		if (chr_func::isDie(it->first) || it->first->m_PlayerInfo.number == m_PlayerInfo.number){
			outcounter++;
			continue;
		}

		//�@����p�v�Z
		vf.x = sinf(m_Params.angle);
		vf.z = cosf(m_Params.angle);

		vt = it->first->m_Params.pos - m_Params.pos;
		vt.y = 0;

		cross = vf.z * vt.x - vf.x * vt.z;

		dot = vf.x * vt.x + vf.z * vt.z;
		dot /= vt.Length();

		//�@����p�O�Ȃ�continue
		if (dot<0.707){
			outcounter++;
			continue;
		}

		//�@�������f
		t = m_Params.pos - it->first->m_Params.pos;
		templen = t.LengthSq();
		//�@��ԋ߂������E�ʒu�Z�o
		if (len > templen){
			len = templen;
			temppos = it->first->m_Params.pos;
		}
	}

	//�@�S������p�O,����ł���
	if (characounter == outcounter){
		characounter = 0;
		outcounter = 0;
		return p;
	}

	//�@�z�[�~���O�v�Z
	v1 = (temppos - p.pos);
	v2 = p.move;

	Vector3Cross(axis, v2, v1);
	axis.Normalize();

	//�@�z�[�~���O����
	float angle = acosf(Vector3Dot(v1, v2) / (v1.Length()*v2.Length()));

	if (angle > 0.05f){
		angle = 0.05f;
	}

	D3DXQuaternionRotationAxis(&q, (D3DXVECTOR3*)&axis, angle);
	D3DXQuaternionConjugate(&invq, &q);

	qpos.x = v2.x;
	qpos.y = v2.y;
	qpos.z = v2.z;
	qpos.w = 0;

	qpos = invq*qpos*q;

	Vector3 m(qpos.x, 0, qpos.z);
	m.Normalize();

	//�@�����x����
	if (acc <= 0.36f){
		acc += 0.018f;
	}
	//�@���ȏ�ł���ɉ���&�z�[�~���O�J�n
	else{
		acc += 0.04f;
		p.move = m*acc;
	}
	//�@�ő�����x
	if (acc >= MaxAcceleration){
		acc = MaxAcceleration;
	}

	//�@�J�E���^�[���Z�b�g
	characounter = 0;
	outcounter = 0;

	return p;
}