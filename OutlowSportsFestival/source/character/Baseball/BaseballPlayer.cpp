#include "BaseballPlayer.h"
#include "../../Damage/Damage.h"
#include "../../GameSystem/GameController.h"
#include "../../Ball/UsualBall.h"

#include "../CharacterFunction.h"
#include "../CharacterManager.h"

//*************************************************************
//		野球プレイヤークラス
//*************************************************************

//　コンストラクタ
BaseballPlayer::BaseballPlayer(const CharacterBase::PlayerInfo& info) :
CharacterBase(info), batterflg(true), acc(0.2f), outcounter(0), characounter(0), m_ModelSize(0.05f),
vf(0, 0, 0), vt(0, 0, 0), cross(0), dot(0), v(0, 0, 0), len(2000.0f), templen(0.0f), temppos(0, 0, 0), num(0),
m_Renderer(new  BlendAnimationMesh("DATA\\CHR\\BaseBall\\player_B.iem"))
{
	m_pStateMachine = new BaseballStateMachine(this);
}

//　デストラクタ
BaseballPlayer::~BaseballPlayer(){
	delete m_pStateMachine;
}

//　ステートセット
void BaseballPlayer::SetState(BaseballState* state){
	//　新しいステートをセット
	m_pStateMachine->set_state(state);
}

//　更新
bool BaseballPlayer::Update(){
	//　切り替え
	Change();
	// ステート実行
	m_pStateMachine->state_execute();

	return true;	//常にtrueを返すと消去されない
}

bool  BaseballPlayer::Msg(MsgType mt){
	return m_pStateMachine->Msg(mt);
}

//　切り替え
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

//　遠距離関数化
BallBase::Params BaseballPlayer::BaseballShot(BaseballPlayer* b, BallBase::Params p, float speed){
	//移動は前向き
	chr_func::GetFront(b, &p.move);
	//スピードは適当
	p.move *= speed;
	//キャラの場所に(最終的に腕の位置に？)
	p.pos = b->m_Params.pos;
	//高さをキャラ共通ボール発射のYに
	p.pos.y = BallBase::UsualBallShotY;
	//親を自分に
	p.pParent = b;
	//通常タイプ
	p.type = BallBase::Type::_Usual;

	return p;
}

//　視野角内計算
Vector3 BaseballPlayer::AngleField(Vector3 t,float range){
	//　map代入
	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();

	//キャラクタ座標を算出
	for (auto it = chr_map.begin(); it != chr_map.end(); ++it){
		characounter++;
		//　死んでたらcontinue
		if (chr_func::isDie(it->first) || it->first->m_PlayerInfo.number == m_PlayerInfo.number){
			outcounter++;
			continue;
		}

		//　視野角計算
		vf.x = sinf(m_Params.angle);
		vf.z = cosf(m_Params.angle);

		vt = it->first->m_Params.pos - m_Params.pos;
		vt.y = 0;

		cross = vf.z * vt.x - vf.x * vt.z;

		dot = vf.x * vt.x + vf.z * vt.z;
		dot /= vt.Length();

		//　視野角外ならcontinue
		if (dot<range){
			outcounter++;
			continue;
		}

		//　距離判断
		t = m_Params.pos - it->first->m_Params.pos;
		templen = t.LengthSq();
		//　一番近い距離・位置算出
		if (len > templen){
			len = templen;
			//　pos保存
			temppos = it->first->m_Params.pos;
			//　number保存
			num = it->first->m_PlayerInfo.number;
		}
	}

	return temppos;
}

//　ホーミング計算
BallBase::Params BaseballPlayer::Homing(BallBase::Params p, Vector3 t){
	BallBase::Params params = p;
	//　ホーミング計算
	v1 = (t - params.pos);
	v2 = params.move;

	Vector3Cross(axis, v2, v1);
	axis.Normalize();

	//　ホーミング方向
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

	//　加速度増加
	if (acc <= 0.36f){
		acc += 0.018f;
	}
	//　一定以上でさらに加速&ホーミング開始
	else{
		acc += 0.04f;
		params.move = m*acc;
	}
	//　最大加速度
	if (acc >= MaxAcceleration){
		acc = MaxAcceleration;
	}

	return params;
}

//　遠距離ターゲット選定
BallBase::Params BaseballPlayer::TargetDecision(BallBase::Params p, Vector3 t){

	//　視野角内計算
	target=AngleField(t,0.707f);

	//　全員視野角外,死んでいる
	if (characounter == outcounter){
		characounter = 0;
		outcounter = 0;
		//target=Vector3(0, 0, 0);
		return p;
	}

	//　ホーミング計算
	tempparam=Homing(p,target);

	//　カウンターリセット
	characounter = 0;
	outcounter = 0;

	return tempparam;
}