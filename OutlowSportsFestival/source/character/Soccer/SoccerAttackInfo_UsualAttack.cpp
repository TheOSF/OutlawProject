#include "SoccerAttackInfo_UsualAttack.h"
#include "../CharacterFunction.h"
#include "../../Sound/Sound.h"

#include "../CharacterManager.h"

#include "../../GameSystem/GameController.h"


SoccerAttackInfo_UsualAtk::SoccerAttackInfo_UsualAtk(
	SoccerPlayer* pOwner
	) :
	m_pOwner(pOwner),
	m_HitStopCount(0)
{

}

SoccerAttackInfo_UsualAtk::~SoccerAttackInfo_UsualAtk()
{

}

//ダメージパラメータをセット
void SoccerAttackInfo_UsualAtk::DamageParamSet(DamageShpere* pDmg)
{
	pDmg->HitCount = 0;
	pDmg->m_Param.size = m_Param.DamageSize;
	pDmg->pBall = nullptr;
	pDmg->pParent = m_pOwner;
	pDmg->type = m_Param.DamageType;
	pDmg->Value = m_Param.DamageValue;

	DamagePosSet(pDmg, m_pOwner);
}


//ダメージ位置をセット
void SoccerAttackInfo_UsualAtk::DamagePosSet(DamageShpere* pDmg, SoccerPlayer* pTennis)
{
	Matrix  BoneMat;
	Vector3 Forward;
	Vector3 Pos;

	m_pOwner->m_Renderer.GetWorldBoneMatirx(BoneMat, m_Param.LocusBoneNum);

	Pos = Vector3(BoneMat._41, BoneMat._42, BoneMat._43);

	Forward = Vector3(BoneMat._31, BoneMat._32, BoneMat._33);
	Forward.Normalize();

	pDmg->m_Param.pos = Pos + Forward * m_Param.DamagePosLength;

	// pDmg->vec = pDmg->m_Param.pos - pTennis->m_Params.pos;
	chr_func::GetFront(pTennis, &pDmg->m_Vec);
    pDmg->m_Vec.y = 0.5f;
	pDmg->m_VecPower.x = 0.25f;


	m_LocusPos = pDmg->m_Param.pos;
	m_LocusVec = Forward;
}


//モーションをセット
void SoccerAttackInfo_UsualAtk::MotionSet(SoccerPlayer* pTennis)
{
	pTennis->m_Renderer.SetMotion(m_Param.Motion);
}


//引数のフレームが攻撃終了かどうか
bool SoccerAttackInfo_UsualAtk::isEnd(int Frame)
{
	return Frame >= m_Param.AllFrame;
}


//引数のフレームがコンボ移行フレームかどうか
bool SoccerAttackInfo_UsualAtk::isComboSwitchFrame(int Frame)
{
	return
		Frame >= m_Param.ComboSwitchStartFrame&&
		Frame <= m_Param.ComboSwitchEndFrame;
}


//引数のフレームが、コンボの移行判定ボタンを押せるフレームかどうか
bool SoccerAttackInfo_UsualAtk::isComboButtonFrame(int Frame)
{
	return Frame >= m_Param.CanComboButtonPushFrame;
}


//引数のフレームが、角度調整を行えるフレームかどうか
bool SoccerAttackInfo_UsualAtk::isCanAngleControll(int Frame, RADIAN& OutControllVal)
{
	OutControllVal = m_Param.AngleControllValue;

	return
		Frame >= m_Param.AngleControllStartFrame&&
		Frame <= m_Param.AngleControllEndFrame;
}


//引数のフレームが、ダメージ判定有効フレームかどうか
bool SoccerAttackInfo_UsualAtk::isDamageEnable(int Frame)
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
void SoccerAttackInfo_UsualAtk::Update(int Frame, Locus* pLocus)
{
	pLocus->AddPoint(m_LocusPos, m_LocusVec);

	chr_func::XZMoveDown(m_pOwner, 0.1f);

	if (Frame >= m_Param.MoveStartFrame&&
		Frame <= m_Param.MoveEndFrame)
	{
		chr_func::AddMoveFront(m_pOwner, m_Param.MoveSpeed, 1000);
	}
}
//ヒットストップ時
void SoccerAttackInfo_UsualAtk::HitStopUpdate()
{
	m_HitStopCount = max(0, m_HitStopCount - 1);
}
//攻撃があたったときに呼ばれる
void SoccerAttackInfo_UsualAtk::HitAttack(DamageShpere* pDmg)
{
	chr_func::AddSkillGauge(m_pOwner, pDmg->Value*0.01f);

    //コントローラを振動
    chr_func::SetControllerShock(
        m_pOwner,
        0.5f,
        0.15f
        );
}//前方に敵がいるかどうか
bool SoccerAttackInfo_UsualAtk::isFrontStayEnemy()
{
	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();
	Vector3 front, vec;

	const float OKlen = 3.0f;
	const RADIAN OKrad = D3DXToRadian(45);

	chr_func::GetFront(m_pOwner, &front);

	for (auto& it : chr_map)
	{
		if (chr_func::isDie(it.first) ||
			it.first == m_pOwner)
		{
			continue;
		}


		vec = it.first->m_Params.pos - m_pOwner->m_Params.pos;

		if (vec.Length() < OKlen && Vector3Radian(vec, front) < OKrad)
		{
			return true;
		}
	}

	return false;
}


//ヒットストップかどうか
bool SoccerAttackInfo_UsualAtk::isHitStopFrame()
{
	return m_HitStopCount > 0;
}

//カウンターヒットするかどうか
bool SoccerAttackInfo_UsualAtk::isCounterFrame(int Timer)
{
    return Timer < m_Param.DamageEnableEnd;
}