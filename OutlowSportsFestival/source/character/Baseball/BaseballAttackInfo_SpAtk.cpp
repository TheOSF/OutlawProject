#include "BaseballAttackInfo_SpAtk.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"

BaseballAttackInfo_SpAtk::BaseballAttackInfo_SpAtk(
	BaseballPlayer* pOwner
	) :
	m_pOwner(pOwner)
{

}

BaseballAttackInfo_SpAtk::~BaseballAttackInfo_SpAtk()
{

}

//�_���[�W�p�����[�^���Z�b�g
void BaseballAttackInfo_SpAtk::DamageParamSet(DamageShpere* pDmg)
{
	pDmg->HitCount = 0;
	pDmg->m_Param.size = m_Param.DamageSize;
	pDmg->pBall = nullptr;
	pDmg->pParent = m_pOwner;
	pDmg->type = m_Param.DamageType;
	pDmg->Value = m_Param.DamageValue;

	DamagePosSet(pDmg, m_pOwner);
}


//�_���[�W�ʒu���Z�b�g
void BaseballAttackInfo_SpAtk::DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pTennis)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;

	m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();

	pDmg->m_Param.pos = Pos + Forward*m_Param.DamagePosLength;

	// pDmg->vec = pDmg->m_Param.pos - pTennis->m_Params.pos;
	chr_func::GetFront(pTennis, &pDmg->vec);
	pDmg->vec.y = 0;

	m_LocusPos = pDmg->m_Param.pos;
	m_LocusVec = Forward;
}


//���[�V�������Z�b�g
void BaseballAttackInfo_SpAtk::MotionSet(BaseballPlayer* pTennis)
{
	pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//�����̃t���[�����U���I�����ǂ���
bool BaseballAttackInfo_SpAtk::isEnd(int Frame)
{
	return Frame >= m_Param.AllFrame;
}








//�����̃t���[�����A�p�x�������s����t���[�����ǂ���
bool BaseballAttackInfo_SpAtk::isCanAngleControll(int Frame, RADIAN& OutControllVal)
{
	OutControllVal = m_Param.AngleControllValue;

	return
		Frame >= m_Param.AngleControllStartFrame&&
		Frame <= m_Param.AngleControllEndFrame;
}


//�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
bool BaseballAttackInfo_SpAtk::isDamageEnable(int Frame)
{
	if (Frame == m_Param.DamageEnableStart)
	{
		Sound::Play(Sound::Swing2);
	}
	return
		Frame >= m_Param.DamageEnableStart&&
		Frame <= m_Param.DamageEnableEnd;
}


//���t���[���Ă΂��
void BaseballAttackInfo_SpAtk::Update(int Frame, LocusHDR* pLocus)
{
	pLocus->AddPoint(m_LocusPos, m_LocusVec);

	chr_func::XZMoveDown(m_pOwner, 0.1f);

	if (Frame >= m_Param.MoveStartFrame&&
		Frame <= m_Param.MoveEndFrame)
	{
		chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
	}
}