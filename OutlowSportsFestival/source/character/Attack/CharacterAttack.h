#ifndef __CHARACTER_ATTACK_CLASS_H__
#define __CHARACTER_ATTACK_CLASS_H__

class CharacterBase;

#include "iextreme.h"
#include "Damage/Damage.h"

//****************************************************************
//		キャラクタ共通の攻撃クラス
//****************************************************************


//通常攻撃クラス
class CharacterNearAttack
{
public:
	struct Params
	{
		int   AttackFrame;  //攻撃開始フレーム
		int   EndFrame;  //攻撃終了フレーム
		float speed;        //スピード(残存)
		float damage;       //ダメージ
		float TurnSpeed;	//角度変化スピード
		int    CanDoComboFrame;       // 次のコンボ受付フレーム
		int    GoNextComboFrame;      // コンボを実行するフレーム
		float  HitCenter;//攻撃の発生中心点
		int   CanHitNum;//最大何ヒットするか
	};

	class AttackEvent
	{
	public:
		virtual~AttackEvent(){}
		virtual void Update() = 0;	//毎フレーム実行される関数
		virtual void Assault() = 0;
		virtual void AttackStart() = 0;			//攻撃開始時に呼ばれる関数
		virtual void AttackEnd() = 0;			//棒立ち開始時に呼ばれる関数
	};

	CharacterNearAttack(
		CharacterBase*	pParent,	//操るキャラクタのポインタ
		const Params&	param,
		AttackEvent*		pAtttackEvent,
		DamageBase::Type	damage_type,	//ダメージ判定のタイプ
		float				damage_val		//ダメージ量
		);

	~CharacterNearAttack();

	bool Update();	//更新
	// コンボできるかどうか
	bool CanDoCombo()const;

	// コンボ実行できるかどうか
	bool CanGoNextCombo()const;
	bool is_End();	//更新
	void SetStickValue(CrVector2 StickValue);
private:
	CharacterBase*	m_pCharacter;
	AttackEvent*	m_pAttackEvent;
	DamageShpere	m_Damage;
	Params			m_Params;
	int             m_timer;
	Vector2			m_StickValue;
};

#endif