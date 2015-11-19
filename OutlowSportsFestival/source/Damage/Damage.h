#ifndef __DAMAGE_H__
#define __DAMAGE_H__

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include <map>
#include <bitset>
#include <array>
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
    
    class HitCallBack
    {
    public:
        virtual ~HitCallBack(){}
        virtual void Hit(const SphereParam* sp) = 0;
    };

	enum Type
	{
		_WeekDamage,	//怯みのみのダメージ
		_VanishDamage,	//吹き飛びダメージ
		_UpDamage,		//上に吹き飛ぶ(バレーとか)追撃可能
	};

    enum class Option
    {
        _DontDie,   //死亡しない(必ず１残る
    };

	LpCharacterBase	    pParent;	    //このダメージ判定の元のキャラクタ(親がキャラクタでない場合はnullptrが入っている)
	LpBallBase		    pBall;		    //この判定についているボール(ボールでなければnull)
    HitCallBack*        pCallBackClass; //コールバッククラス(nullの場合はよび出さない)
    Type			    type;		    //このダメージのタイプ
	float			    Value;		    //値
	int				    HitCount;	    //当たった回数
    int                 MaxChrHit;      //１キャラクタに当たる総数

	DamageBase();
	virtual ~DamageBase();

    bool isOptionOn(Option op)const;       //引数のオプションがonかどうか
    void SetOption(Option op, bool flag);  //オプションのon,offをセット

    void ResetCounts();                    //ヒットカウント、キャラヒットカウントをリセット
    void AddHitCount(CharacterBase* pHitChr);            //キャラクタに当たった場合カウントを加算
    bool isCanHitCharacter(CharacterBase* pHitChr)const; //引数のキャラクタに当たれるかどうか

    virtual bool HitCheckSphere(const SphereParam* sp) = 0;
    virtual void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec) = 0;
   
    virtual void DebugDraw() = 0;

private:
    static const int            MaxPlayer = 4;
    std::bitset<16>             m_OptionFlags;
    std::array<int, MaxPlayer>  m_PlayerHitCounts;
};



//*************************************************************
//	ダメージクラス(球)
//*************************************************************
class DamageShpere :public DamageBase
{
public:

    //吹き飛び方向の列挙
    enum class DamageVecType
    {
        MemberParam,     //メンバー変数の方向(m_Vecの方向)
        CenterToPos,     //中心から放射状に
        PosToCenter,     //中心に向かって

        CenterToPosXZ, //中心から放射状に(Y方向は常にm_VecPower.yの値になる)
        PosToCenterXZ, //中心に向かって  (Y方向は常にm_VecPower.yの値になる)
    };

    
	bool			m_Enable;	//このダメージが有効かどうか
    SphereParam		m_Param;	//このダメージの球の構造体

    DamageVecType   m_VecType;  //ダメージ方向タイプ
    Vector3         m_Vec;      //ダメージ方向(m_VecTypeがMemberParamの場合のみ有効)
    Vector2         m_VecPower; //ダメージの吹き飛び速度(x = XZ平面 y=Y軸方向)

	DamageShpere();

    bool HitCheckSphere(const SphereParam* sp);
    void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec);

private:
    void DebugDraw();
};

//*************************************************************
//	ダメージクラス(カプセル)
//*************************************************************
class DamageCapsure :public DamageBase
{
public:
    //吹き飛び方向の列挙
    enum class DamageVecType
    {
        MemberParam,  //メンバー変数の方向(m_Vecの方向)
        CenterToPos,  //中心から放射状に
        PosToCenter,  //中心に向かって

        CenterToPosXZ, //中心から放射状に(Y方向は常にm_VecPower.yの値になる)
        PosToCenterXZ, //中心に向かって  (Y方向は常にm_VecPower.yの値になる)
    };

    bool			m_Enable;	//このダメージが有効かどうか
    CapsureParam	m_Param;	//このダメージの球の構造体

    DamageVecType   m_VecType;  //ダメージ方向タイプ
    Vector3         m_Vec;      //ダメージ方向(m_VecTypeがMemberParamの場合のみ有効)
    Vector2         m_VecPower; //ダメージの吹き飛び速度(x = XZ平面 y=Y軸方向)

    DamageCapsure();

    bool HitCheckSphere(const SphereParam* sp);
    void CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec);

private:
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