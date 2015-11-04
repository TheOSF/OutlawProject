#include "Damage.h"
#include "../debug/DebugDraw.h"
#include "../character/CharacterBase.h"

//ダメージクラス
DamageBase::DamageBase() :
pParent(nullptr),
pBall(nullptr),
type(_WeekDamage),
Value(1),
vec(Vector3Zero),
HitCount(0)
{
#ifdef _DEBUG
	MyAssert(DefDamageMgr.AddDamage(this), "ダメージ登録の失敗");
#else
    DefDamageMgr.AddDamage(this);
#endif
}

DamageBase::~DamageBase()
{
#ifdef _DEBUG
	MyAssert(DefDamageMgr.EraceDamage(this), "ダメージ削除の失敗");
#else
    DefDamageMgr.EraceDamage(this);
#endif
}

DamageShpere::DamageShpere():
m_Enable(true)
{
	m_Param.pos = Vector3Zero;
	m_Param.size = 1;
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


bool DamageShpere::HitCheckCapsure(const CapsureParam* cp)
{
    if (!m_Enable)
    {
        return false;
    }

    return 
        isHitSphereCapsure(
            m_Param.pos,
            m_Param.size,
            cp->pos1,
            cp->pos2,
            cp->width
        );
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

bool DamageCapsure::HitCheckCapsure(const CapsureParam* cp)
{
    if (!m_Enable)
    {
        return false;
    }



    return false;
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
		}
	}
}

//カプセルでダメージ判定を取得する
void DamageManager::HitCheckCapsure(
    const CapsureParam&	cp,
    HitEventBase&		HitEvent)
{

    if (m_DebugDrawVisible)
    {
        //デバッグ用球描画(黒)
        new DebugDrawPole(
            cp.pos1,
            cp.pos2,
            cp.width,
            COLORf(0.4f, 0, 0, 0)
            );
    }


    for (auto it = m_DamageBaseMap.begin();
        it != m_DamageBaseMap.end();
        ++it
        )
    {
        //もしあたっていたら
        if (it->second->HitCheckCapsure(&cp) &&
            HitEvent.Hit(it->second))
        {
            //当たった回数カウントを足す
            ++it->second->HitCount;
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