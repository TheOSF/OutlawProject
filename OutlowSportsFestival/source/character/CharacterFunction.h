#ifndef __CHARACTER_FUNCTION_H__
#define __CHARACTER_FUNCTION_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"

class CharacterBase;

//キャラクタ用関数集

namespace chr_func
{
	//座標に移動量を更新する
	void PositionUpdate(CharacterBase* p);
			
	//ＸＺ軸で移動量を足す(max_speedを超えないように設定される)
	void AddXZMove(CharacterBase*p, float x, float z, float max_speed);

	//前方向に移動量をタス
	void AddMoveFront(CharacterBase*p, float speed, float max_speed);

	//徐々に減速する(tを１にすると一瞬で止まる,0で効果なし)
	void XZMoveDown(CharacterBase*p, float t);

	//ＸＺ方向の移動量がmax_speedを超えていたら、max_speedに設定する
	void SetMaxXZspeed(CharacterBase*p, float max_speed);

	//指定した場所に向く
	void AngleControll(CharacterBase* p, CrVector3 view_pos, float speed);

	//指定した場所に向く(一瞬で)
	void AngleControll(CharacterBase* p, CrVector3 view_pos);

	//現在の位置、現在のangle、Scaleをもとに変換行列を生成する
	void CreateTransMatrix(CharacterBase* p, float Scale, Matrix* pOutMatrix);

	//前方向ベクトルを得る
	void GetFront(CharacterBase* p, Vector3* pOut);

	//キャラクタが死んでいるかどうか
	bool isDie(CharacterBase* p);

	//ダメージ判定をとる
	void DamageCheck(
		LpCharacterBase					pCharacter,	//判定をとるキャラクタ
		DamageManager::HitEventBase*	pHitEvent	//イベント処理
		);

    //地面判定をとる(戻り値：地面についているかどうか)
    bool isTouchGround(CharacterBase* p);

    //Y軸方向の速度を更新する
    void UpdateMoveY(CharacterBase* p);

    //Y軸方向の地面判定を行う
    void CheckGround(CharacterBase* p);

    //XZ方向の壁との接触判定を行う(戻り値＝壁とあたっているかどうか)
    bool CheckWall(CharacterBase* p);

    //現在の体力の割合を得る（０～１）
    RATIO GetLifeRatio(CharacterBase* p);
}

#endif