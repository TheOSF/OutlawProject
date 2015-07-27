#include "CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterFunction.h"
#include "../character/CharacterBase.h"

//�������̃J�����X�e�[�g


void CameraStateGamePlay::Enter(Camera* c)
{

}

void CameraStateGamePlay::Execute(Camera* c)
{

	const Vector3 first_pos(0, 40, -55);	//�K��
	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();

	//�L�����N�^�����Ȃ��ꍇ�ړ��ł��Ȃ�
	if (chr_map.empty())
	{
		return;
	}

	Vector3 center(0, 0, 0);
	int livecnt = 0;
	const float viewangle = PI / 4.5f;	//�J�����̎���p
	float dRightLen = 0;

	//�S�L�����N�^���W�̕��ύ��W���Z�o����
	for (auto it = chr_map.begin();
		it != chr_map.end();
		++it)
	{
		if (chr_func::isDie(it->first))continue;
		center += it->first->m_Params.pos;
		livecnt++;
	}

	if (livecnt > 1)
	{
		center /= (float)livecnt;
	}
	center.z -= 4;	//�኱��O��

	//�J�����̃^�[�Q�b�g����
	c->m_Target += (center - c->m_Target)*0.02f;



	//�ǂꂾ���J���������������Z�o����
	for (auto it = chr_map.begin();
		it != chr_map.end();
		++it)
	{
		if (chr_func::isDie(it->first))continue;
		CrVector3 chrpos = it->first->m_Params.pos;

		//�J���������_����̋��������߂�
		float rlen = Vector3Length(chrpos - center);
		rlen += 17;	//������Ƒ傫�߂Ɉ�������

		if (dRightLen < rlen)dRightLen = rlen;	//�ő�Ȃ�X�V
	}

	Vector3 mVec = first_pos - center;
	Vector3 moveTarget;
	mVec.Normalize();

	//����p��tan����ڕW������ɓ���邽�߂̌���������Z�o
	float b = dRightLen / tanf(viewangle);

	//�ړ��ڕW���v�Z
	moveTarget = center + mVec*b;

	//��Ԃ���
	c->m_Position += (moveTarget - c->m_Position)*0.04f;

}

void CameraStateGamePlay::Exit(Camera* c)
{


}



//�ڕW�Ɍ������ĂQ�����ňړ��X�e�[�g

CameraStateMovetoPoint::CameraStateMovetoPoint(
	CrVector3      pos,
	CrVector3      target,
	float          speed,
	UINT           frame,
	CalcType       calcType,
	CameraState*   pNextState
	) :
	m_Pos(pos),
	m_Target(target),
	m_Speed(speed),
	m_Frame(frame),
	m_pNextState(pNextState),
	m_FrameCount(0),
	m_CalcType(calcType)
{

}

CameraStateMovetoPoint::~CameraStateMovetoPoint()
{
	delete m_pNextState;
}

void CameraStateMovetoPoint::Enter(Camera* c)
{

}

void CameraStateMovetoPoint::Execute(Camera* c)
{
	//���`
	if (m_CalcType == CalcType::_Linear)
	{
		Vector3 vec;

		vec = m_Pos - c->m_Position;
		if (vec.Length() > m_Speed)
		{
			vec.Normalize();
			vec *= m_Speed;
		}
		c->m_Position += vec;

		vec = m_Target - c->m_Target;
		if (vec.Length() > m_Speed)
		{
			vec.Normalize();
			vec *= m_Speed;
		}
		c->m_Target += vec;
	}

	//cos��
	if (m_CalcType == CalcType::_Cos)
	{
		c->m_Position += (m_Pos - c->m_Position)*m_Speed;
		c->m_Target += (m_Target - c->m_Target)*m_Speed;
	}


	//�X�e�[�g�ڍs
	if (++m_FrameCount > m_Frame)
	{
		CameraState* tmp = m_pNextState;

		m_pNextState = nullptr;
		c->SetNewState(tmp);
	}
}

void CameraStateMovetoPoint::Exit(Camera* c)
{

}