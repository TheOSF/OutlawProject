#include "BaseballAttackInfo_UsualAtk.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"

#include "../../GameSystem/GameController.h"


BaseballAttackInfo_UsualAtk::BaseballAttackInfo_UsualAtk(
	BaseballPlayer* pOwner
	) :
	m_pOwner(pOwner)
{

}

BaseballAttackInfo_UsualAtk::~BaseballAttackInfo_UsualAtk()
{

}

//ダメージパラメータをセット
void BaseballAttackInfo_UsualAtk::DamageParamSet(DamageShpere* pDmg)
{
	pDmg->HitCount = 0;
	pDmg->m_Param.size = m_Param.DamageSize;
	pDmg->pBall = nullptr;
	pDmg->pParent = m_pOwner;
	pDmg->type = m_Param.DamageType;
	pDmg->Value = m_Param.DamageValue;
	pDmg->m_VecPower = m_Param.VanishValue;
	DamagePosSet(pDmg, m_pOwner);
}


//ダメージ位置をセット
void BaseballAttackInfo_UsualAtk::DamagePosSet(DamageShpere* pDmg, BaseballPlayer* pBaseball)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;

	m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();

	//pDmg->m_Param.pos = Pos + Forward*m_Param.DamagePosLength;
	pDmg->m_Param.pos = pBaseball->m_Params.pos + chr_func::GetFront(pBaseball)*1.5f + Vector3(0, 2, 0);

	// pDmg->vec = pDmg->m_Param.pos - pTennis->m_Params.pos;
    chr_func::GetFront(pBaseball, &pDmg->m_Vec);
    pDmg->m_Vec.y = 1;
	//pDmg->m_VecPower.x = 0.2f;

	m_LocusPos = pDmg->m_Param.pos;
	m_LocusVec = Forward;
}


//モーションをセット
void BaseballAttackInfo_UsualAtk::MotionSet(BaseballPlayer* pTennis)
{
	pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//引数のフレームが攻撃終了かどうか
bool BaseballAttackInfo_UsualAtk::isEnd(int Frame)
{
	return Frame >= m_Param.AllFrame;
}


//引数のフレームがコンボ移行フレームかどうか
bool BaseballAttackInfo_UsualAtk::isComboSwitchFrame(int Frame)
{
	return
		Frame >= m_Param.ComboSwitchStartFrame&&
		Frame <= m_Param.ComboSwitchEndFrame;
}


//引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
bool BaseballAttackInfo_UsualAtk::isComboButtonFrame(int Frame)
{
	return Frame >= m_Param.CanComboButtonPushFrame;
}


//引数のフレームが、角度調整を行えるフレームかどうか
bool BaseballAttackInfo_UsualAtk::isCanAngleControll(int Frame, RADIAN& OutControllVal)
{
	OutControllVal = m_Param.AngleControllValue;

	return
		Frame >= m_Param.AngleControllStartFrame&&
		Frame <= m_Param.AngleControllEndFrame;
}


//引数のフレームが、ダメージ判定有効フレームかどうか
bool BaseballAttackInfo_UsualAtk::isDamageEnable(int Frame)
{
	if (Frame == m_Param.DamageEnableStart)
	{
		Sound::Play(Sound::Swing2);
	}
	return
		Frame >= m_Param.DamageEnableStart&&
		Frame <= m_Param.DamageEnableEnd;
}


//毎フレーム呼ばれる
void BaseballAttackInfo_UsualAtk::Update(int Frame, LocusHDR* pLocus)
{
	pLocus->AddPoint(m_LocusPos, m_LocusVec);

	chr_func::XZMoveDown(m_pOwner, 0.1f);

	if (Frame >= m_Param.MoveStartFrame&&
		Frame <= m_Param.MoveEndFrame)
	{
		chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
	}
}

//攻撃があたったときに呼ばれる
void BaseballAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
	//スキルゲージ増加
	chr_func::AddSkillGauge(m_pOwner, m_Param.AddSkillValue);

	//コントローラを振動
    chr_func::SetControllerShock(
        m_pOwner,
        0.5f,
        0.15f
        );
	m_pOwner->setChangeFlg(m_Param.changeFlg);
}


//カウンターヒットフレームかどうか
bool BaseballAttackInfo_UsualAtk::isCounterHitFrame(int Frame)
{
    return Frame < m_Param.DamageEnableEnd;
}