#include "Damage.h"
#include "../debug/DebugDraw.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"

//ダメージクラス
DamageBase::DamageBase() :
pParent(nullptr),
pBall(nullptr),
pCallBackClass(nullptr),
type(_WeekDamage),
Value(1),
HitCount(0),
m_OptionFlags(),
MaxChrHit(1),
HitMotionFrame(25),
AddSkillGaugeValue(2)
{
#ifdef _DEBUG
	MyAssert(DefDamageMgr.AddDamage(this), "ダメージ登録の失敗");
#else
    DefDamageMgr.AddDamage(this);
#endif
    m_PlayerHitCounts.fill(0);
}

DamageBase::~DamageBase()
{
    delete pCallBackClass;
    pCallBackClass = nullptr;

#ifdef _DEBUG
	MyAssert(DefDamageMgr.EraceDamage(this), "ダメージ削除の失敗");
#else
    DefDamageMgr.EraceDamage(this);
#endif
}

bool DamageBase::isOptionOn(Option op)const
{
    //引数のオプションがonかどうか
    return m_OptionFlags.at((int)op);
}
void DamageBase::SetOption(Option op, bool flag)
{
    //オプションのon,offをセット
    m_OptionFlags.at((int)op) = flag;
}

void DamageBase::ResetCounts()
{
    //ヒットカウント、キャラヒットカウントをリセット
    m_PlayerHitCounts.fill(0);
    HitCount = 0;
}

void DamageBase::OnHitCharacter(CharacterBase* pHitChr)
{
    //キャラクタに当たった場合カウントを加算
    ++m_PlayerHitCounts.at((int)pHitChr->m_PlayerInfo.number);

    //スキルゲージ加算
    if (pParent != nullptr)
    {
        chr_func::AddSkillGauge(pParent, Value * AddSkillGaugeValue * 0.01f);
    }
}

bool DamageBase::isCanHitCharacter(CharacterBase* pHitChr)const
{
    //引数のキャラクタに当たれるかどうか
    return m_PlayerHitCounts.at((int)pHitChr->m_PlayerInfo.number) < MaxChrHit;
}


DamageShpere::DamageShpere():
m_Enable(true)
{
	m_Param.pos = Vector3Zero;
	m_Param.size = 1;

    m_VecType = DamageVecType::MemberParam;
    m_Vec = Vector3(0, 1, 0);
    m_VecPower = Vector2(1, 1)*0.1f;
}

//ダメージクラス(球)
bool DamageShpere::HitCheckSphere(const SphereParam* sp)
{
	if (!m_Enable)
	{
		return false;
	}
	const float L = Vector3Length(sp->pos - this->m_Param.pos);

	return L < (sp->size + this->m_Param.size);
}

void DamageShpere::CalcPosVec(
    CrVector3 hit_pos,
    Vector3* pOutPos,
    Vector3* pOutVec)
{
    //ダメージ位置は球の中心
    *pOutPos = m_Param.pos;

    //方向
    switch (m_VecType)
    {
    case DamageVecType::MemberParam:
        //固定値
        *pOutVec = m_Vec;
        break;

    case DamageVecType::CenterToPos:
    case DamageVecType::CenterToPosXZ:
        //放射状
        *pOutVec = hit_pos - m_Param.pos;
        break;

    case DamageVecType::PosToCenter:
    case DamageVecType::PosToCenterXZ:
        //中心に向かって
        *pOutVec = m_Param.pos - hit_pos;
        break;

    default:
        MyAssert(false, "m_VecTypeが不正です type = %d ", (int)m_VecType);
        *pOutVec = Vector3AxisX;
        break;
    }


    //Ｙ軸移動の固定の場合
    if (m_VecType == DamageVecType::CenterToPosXZ || 
        m_VecType == DamageVecType::PosToCenterXZ )
    {
        const float l = Vector3XZLength(*pOutVec);

        pOutVec->x /= l;
        pOutVec->z /= l;
        pOutVec->y = 1.0f;
    }
    else
    {
        //正規化
        *pOutVec = Vector3Normalize(*pOutVec);
    }

    //方向パワーをかける
    pOutVec->x *= m_VecPower.x;
    pOutVec->z *= m_VecPower.x;

    pOutVec->y *= m_VecPower.y;

}


void DamageShpere::DebugDraw()
{
    COLORf color(0.4f, 1, 1, 1);

    if (!m_Enable)
    {
        return;
    }

    if (pParent)
    {
        color.SetColor(CharacterBase::GetPlayerColor(pParent->m_PlayerInfo.number));
        color.a = 0.4f;
    }

    new DebugDrawSphere(
        m_Param.pos,
        m_Param.size,
        color
        );
}

DamageCapsure::DamageCapsure():
m_Enable(true)
{
    m_Param.pos1 = Vector3Zero;
    m_Param.pos2 = Vector3(0, 10, 0);

    m_VecType = DamageVecType::MemberParam;
    m_Vec = Vector3(0, 1, 0);
    m_VecPower = Vector2(1, 1);
}

//ダメージクラス(カプセル)
bool DamageCapsure::HitCheckSphere(const SphereParam* sp)
{
    if (!m_Enable)
    {
        return false;
    }


    return
        isHitSphereCapsure(
            sp->pos,
            sp->size,
            m_Param.pos1,
            m_Param.pos2,
            m_Param.width
        );
}

void DamageCapsure::CalcPosVec(
    CrVector3 hit_pos,
    Vector3* pOutPos,
    Vector3* pOutVec)
{
    //最近傍点の算出
    {
        Vector3 v1 = hit_pos - m_Param.pos1;
        Vector3 v2 = m_Param.pos1 - m_Param.pos2;

        float v2l = v2.Length();

        v2 /= v2l;

        float l = Vector3Dot(v1, v2);

        if (l < 0)
        {
            l = 0;
        }
        else if (l > v2l)
        {
            l = v2l;
        }

        *pOutPos = m_Param.pos1 + v2*l;
    }


    //方向
    {
        
        //方向
        switch (m_VecType)
        {
        case DamageVecType::MemberParam:
            //固定値
            *pOutVec = m_Vec;
            break;

        case DamageVecType::CenterToPos:
        case DamageVecType::CenterToPosXZ:
            //放射状
            *pOutVec = hit_pos - *pOutPos;
            break;

        case DamageVecType::PosToCenter:
        case DamageVecType::PosToCenterXZ:
            //中心に向かって
            *pOutVec = *pOutPos - hit_pos;
            break;

        default:
            MyAssert(false, "m_VecTypeが不正です type = %d ", (int)m_VecType);
            *pOutVec = Vector3AxisX;
            break;
        }

        //Ｙ軸移動の固定の場合
        if (m_VecType == DamageVecType::CenterToPosXZ ||
            m_VecType == DamageVecType::PosToCenterXZ)
        {
            const float l = Vector3XZLength(*pOutVec);

            pOutVec->x /= l;
            pOutVec->z /= l;
            pOutVec->y = 1.0f;
        }
        else
        {
            //正規化
            *pOutVec = Vector3Normalize(*pOutVec);
        }

        //方向パワーをかける
        pOutVec->x *= m_VecPower.x;
        pOutVec->z *= m_VecPower.x;

        pOutVec->y *= m_VecPower.y;
    }
}

void DamageCapsure::DebugDraw()
{
    COLORf color(0.4f, 1, 1, 1);

    if (!m_Enable)
    {
        return;
    }

    if (pParent)
    {
        color.SetColor(CharacterBase::GetPlayerColor(pParent->m_PlayerInfo.number));
        color.a = 0.4f;
    }

    new DebugDrawPole(
        m_Param.pos1,
        m_Param.pos2,
        m_Param.width,
        color
        );
}

//*************************************************************
//	姿勢制御ダメージクラス(形状は球)
//*************************************************************


//コンストラクタ
DamageControllVanish::DamageControllVanish(
    GetDamageControllTransformClass*     pGetDamageControllClass  //デストラクタでdeleteする
    ):
    m_pGetDamageControllClass(pGetDamageControllClass),
    m_Enable(true)
{

}

DamageControllVanish::~DamageControllVanish()
{
    delete m_pGetDamageControllClass;
}

bool DamageControllVanish::HitCheckSphere(const SphereParam* sp)
{
    if (!m_Enable)
    {
        return false;
    }
    const float L = Vector3Length(sp->pos - this->m_Param.pos);

    return L < (sp->size + this->m_Param.size);
}

void DamageControllVanish::CalcPosVec(CrVector3 hit_pos, Vector3* pOutPos, Vector3* pOutVec)
{
    *pOutPos = hit_pos;
    *pOutVec = Vector3Normalize(hit_pos - m_Param.pos);
}

DamageControll_Transform* DamageControllVanish::GetDamageControll_Transform()
{
    return m_pGetDamageControllClass->Get();
}

void DamageControllVanish::DebugDraw()
{
    COLORf color(0.75f, 0, 0, 0);

    if (!m_Enable)
    {
        return;
    }

    if (pParent)
    {
        color.SetColor(CharacterBase::GetPlayerColor(pParent->m_PlayerInfo.number));
        color.a = 0.4f;
    }

    new DebugDrawSphere(
        m_Param.pos,
        m_Param.size,
        color
        );
}

//*************************************************************
//		ダメージ判定マネージャ
//*************************************************************

DamageManager* DamageManager::m_pInstance = nullptr;

DamageManager& DamageManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new DamageManager();
	}

	return *m_pInstance;
}

void DamageManager::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}


//球のダメージ判定をとる
void DamageManager::HitCheckSphere(
    const SphereParam&	sp,
	HitEventBase&		HitEvent
	)
{

    if (m_DebugDrawVisible)
    {
        //デバッグ用球描画(黒)
        new DebugDrawSphere(
            sp.pos,
            sp.size,
            COLORf(0.4f, 0, 0, 0)
            );
    }


	for (auto it = m_DamageBaseMap.begin();
		it != m_DamageBaseMap.end();
		++it
		)
	{
		//もしあたっていたら
		if (it->second->HitCheckSphere(&sp) &&
			HitEvent.Hit(it->second))
		{
			//当たった回数カウントを足す
			++it->second->HitCount;
            
            //コールバック関数呼び出し
            if (it->second->pCallBackClass != nullptr)
            {
                it->second->pCallBackClass->Hit(&sp);
            }
            break;
		}
	}
}


//あたり判定をデバッグ描画
void DamageManager::DebugDraw()
{
    if (!m_DebugDrawVisible)
    {
        return;
    }

    for (auto it = m_DamageBaseMap.begin();
         it != m_DamageBaseMap.end();
         ++it
         )
    {
        it->first->DebugDraw();
    }
}

DamageManager::DamageManager() :
m_DebugDrawVisible(false)
{

}

DamageManager::~DamageManager()
{

}


bool DamageManager::AddDamage(DamageBase* pDmg)
{
	if (m_DamageBaseMap.find(pDmg) != m_DamageBaseMap.end())
	{
		return false;
	}

	m_DamageBaseMap.insert(
		DamageBaseMap::value_type(pDmg, pDmg));

	return true;
}


bool DamageManager::EraceDamage(DamageBase* pDmg)
{
	auto it = m_DamageBaseMap.find(pDmg); 

	if (it == m_DamageBaseMap.end())
	{
		return false;
	}

	m_DamageBaseMap.erase(it);

	return true;
}