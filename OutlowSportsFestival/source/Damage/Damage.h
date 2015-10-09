#ifndef __DAMAGE_H__
#define __DAMAGE_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include <map>

//*************************************************************
//		ダメージ判定クラスヘッダ
//*************************************************************


//球の構造体
struct SphereParam
{
	Vector3		pos;
	float		size;
};

//カプセルの構造体
struct CapsureParam
{
    float       width;
    Vector3		pos1;
    Vector3		pos2;
};

//**************************************************************************
//	ダメージクラス(コンストラクタ・デストラクタで自動的に登録・削除をする)
//**************************************************************************
class DamageBase
{
public:
	enum Type
	{
		_WeekDamage,	//怯みのみのダメージ
		_VanishDamage,	//吹き飛びダメージ
		_UpDamage,		//上に吹き飛ぶ(バレーとか)追撃可能
	};

	LpCharacterBase	pParent;	//このダメージ判定の元のキャラクタ
	LpBallBase		pBall;		//この判定についているボール(ボールでなければnull)
	Type			type;		//このダメージのタイプ
	float			Value;		//値
	Vector3			vec;		//ひるむ方向ベクトル
	int				HitCount;	//当たった回数

	DamageBase();
	virtual ~DamageBase();

    virtual bool HitCheckSphere(const SphereParam* sp) = 0;
    virtual bool HitCheckCapsure(const CapsureParam* cp) = 0;

    virtual void DebugDraw() = 0;
};


//*************************************************************
//	ダメージクラス(球)
//*************************************************************
class DamageShpere :public DamageBase
{
public:
	bool			m_Enable;	//このダメージが有効かどうか
    SphereParam		m_Param;	//このダメージの球の構造体

	DamageShpere();
private:
    bool HitCheckSphere(const SphereParam* sp);
    bool HitCheckCapsure(const CapsureParam* cp);

    void DebugDraw();
};

//*************************************************************
//	ダメージクラス(カプセル)
//*************************************************************
class DamageCapsure :public DamageBase
{
public:
    bool			m_Enable;	//このダメージが有効かどうか
    CapsureParam	m_Param;	//このダメージの球の構造体

    DamageCapsure();
private:
    bool HitCheckSphere(const SphereParam* sp);
    bool HitCheckCapsure(const CapsureParam* cp);
    void DebugDraw();
};








//*************************************************************
//		ダメージ判定マネージャ
//*************************************************************

class DamageManager
{
public:
	//インスタンスゲッタ
	static DamageManager& GetInstance();
	static void Release();

	//ダメージヒット時のイベントクラス
	class HitEventBase
	{
	public:
		virtual ~HitEventBase(){}
		virtual bool Hit(DamageBase* pDmg){ return false; }	//当たった時に呼ばれる関数(戻り値：当たったかどうか)
	};

	//球でダメージ判定を取得する
	void HitCheckSphere(
        const SphereParam&	sp,
		HitEventBase&		HitEvent
		);

    //カプセルでダメージ判定を取得する
    void HitCheckCapsure(
        const CapsureParam&	cp,
        HitEventBase&		HitEvent);

    //あたり判定をデバッグ描画
    void DebugDraw();

    //デバッグ描画を実行するかどうか
    bool m_DebugDrawVisible;

private:
	friend class DamageBase;
	typedef std::map<DamageBase*, DamageBase*> DamageBaseMap;
	
	static DamageManager*	m_pInstance;
	DamageBaseMap			m_DamageBaseMap;
	
	DamageManager();
	~DamageManager();

	bool AddDamage(DamageBase* pDmg);
	bool EraceDamage(DamageBase* pDmg);
};

typedef DamageManager::HitEventBase HitEventBase;

#define DefDamageMgr (DamageManager::GetInstance())

#endif