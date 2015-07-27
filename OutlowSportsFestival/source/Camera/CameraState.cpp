#include "CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterFunction.h"
#include "../character/CharacterBase.h"

//試合時のカメラステート


void CameraStateGamePlay::Enter(Camera* c)
{

}

void CameraStateGamePlay::Execute(Camera* c)
{

	const Vector3 first_pos(0, 40, -55);	//適当
	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();

	//キャラクタがいない場合移動できない
	if (chr_map.empty())
	{
		return;
	}

	Vector3 center(0, 0, 0);
	int livecnt = 0;
	const float viewangle = PI / 4.5f;	//カメラの視野角
	float dRightLen = 0;

	//全キャラクタ座標の平均座標を算出する
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
	center.z -= 4;	//若干手前に

	//カメラのターゲットを補間
	c->m_Target += (center - c->m_Target)*0.02f;



	//どれだけカメラを引くかを算出する
	for (auto it = chr_map.begin();
		it != chr_map.end();
		++it)
	{
		if (chr_func::isDie(it->first))continue;
		CrVector3 chrpos = it->first->m_Params.pos;

		//カメラ注視点からの距離を求める
		float rlen = Vector3Length(chrpos - center);
		rlen += 17;	//ちょっと大きめに引くため

		if (dRightLen < rlen)dRightLen = rlen;	//最大なら更新
	}

	Vector3 mVec = first_pos - center;
	Vector3 moveTarget;
	mVec.Normalize();

	//視野角のtanから目標を視野に入れるための後方距離を算出
	float b = dRightLen / tanf(viewangle);

	//移動目標を計算
	moveTarget = center + mVec*b;

	//補間する
	c->m_Position += (moveTarget - c->m_Position)*0.04f;

}

void CameraStateGamePlay::Exit(Camera* c)
{


}



//目標に向かって２次式で移動ステート

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
	//線形
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

	//cos式
	if (m_CalcType == CalcType::_Cos)
	{
		c->m_Position += (m_Pos - c->m_Position)*m_Speed;
		c->m_Target += (m_Target - c->m_Target)*m_Speed;
	}


	//ステート移行
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