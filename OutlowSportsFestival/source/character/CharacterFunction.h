#ifndef __CHARACTER_FUNCTION_H__
#define __CHARACTER_FUNCTION_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"

class CharacterBase;

//キャラクタ用関数集

namespace chr_func
{
    //基本的な更新(座標更新、壁との判定など)をすべて行う
    void UpdateAll(CharacterBase* p, DamageManager::HitEventBase*	pHitEvent);

    //基本的な更新(座標更新、壁との判定など)をすべて行う
    void UpdateAll(CharacterBase* p, DamageManager::HitEventBase*	pHitEvent, RATIO Speed);

	//座標に移動量を更新する
    void PositionUpdate(CharacterBase* p, RATIO t = 1.0f);
			
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
	void    GetFront(CharacterBase* p, Vector3* pOut);
    Vector3 GetFront(CharacterBase* p);

    //右方向ベクトルを得る
    void    GetRight(CharacterBase* p, Vector3* pOut);
    Vector3 GetRight(CharacterBase* p);

	//キャラクタが死んでいるかどうか
	bool isDie(const CharacterBase* p);

	//ダメージ判定をとる
	void DamageCheck(
		LpCharacterBase					pCharacter,	//判定をとるキャラクタ
		DamageManager::HitEventBase*	pHitEvent	//イベント処理
		);

    //地面判定をとる(戻り値：地面についているかどうか)
    bool isTouchGround(CharacterBase* p);

    //Y軸方向の速度を更新する
    void UpdateMoveY(CharacterBase* p, RATIO t = 1.0f);

    //Y軸方向の地面判定を行う
    void CheckGround(CharacterBase* p);

    //XZ方向の壁との接触判定を行う(戻り値＝壁とあたっているかどうか)
    bool CheckWall(CharacterBase* p);

    //現在の体力の割合を得る（０～１）
    RATIO GetLifeRatio(CharacterBase* p);

    //移動量をゼロにする
    void ResetMove(CharacterBase* p);

    //引数の場所が引数のキャラクタからみて右かどうか
    bool isRight(CharacterBase* p, CrVector3 pos);

    //引数の場所が引数のキャラクタからみて前かどうか
    bool isFront(CharacterBase* p, CrVector3 pos);

    //引数のキャラクタのゲージを加算する
    void AddSkillGauge(CharacterBase* p, RATIO value);

    //引数のキャラクタのゲージをリセットする(０にする)
    void ResetSkillGauge(CharacterBase* p);

    //引数のスキル値が必殺技を発動できるかどうか
    bool isCanSpecialAttack(RATIO value);

    //引数のキャラクタが必殺技を発動できるかどうか
    bool isCanSpecialAttack(CharacterBase* p);

    //引数のキャラクタのＳＰが０かどうか
    bool isSkillZero(CharacterBase* p);

    //キャラクタの体力をダメージによって減少させる
    void CalcDamage(CharacterBase* p, float value, bool DontDie = false);

    //攻撃のターゲットを得る(複数のターゲットがいた場合、自身のもっとも前にいるキャラクタを選ぶ)
    bool CalcAtkTarget(CharacterBase* pOwner, RADIAN check_angle, float check_distance, CharacterBase** ppOut);

    //キャラクタのラウンド勝利数を加算する
    void AddWinPoint(CharacterBase* p);
}

#endif