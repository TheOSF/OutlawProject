#include "Damage.h"
#include "../debug/DebugDraw.h"
#include "../character/CharacterBase.h"

//�_���[�W�N���X
DamageBase::DamageBase() :
pParent(nullptr),
pBall(nullptr),
type(_WeekDamage),
Value(1),
vec(Vector3Zero),
HitCount(0)
{
	MyAssert(DefDamageMgr.AddDamage(this), "�_���[�W�o�^�̎��s");
}

DamageBase::~DamageBase()
{
	MyAssert(DefDamageMgr.EraceDamage(this), "�_���[�W�폜�̎��s");
}

DamageShpere::DamageShpere():
m_Enable(true)
{
	m_Param.pos = Vector3Zero;
	m_Param.size = 1;
}

//�_���[�W�N���X(��)
bool DamageShpere::HitCheckSphere(const ShpereParam* sp)
{
	if (!m_Enable)
	{
		return false;
	}
	const float L = Vector3Length(sp->pos - this->m_Param.pos);

	return L < (sp->size + this->m_Param.size);
}

void DamageShpere::DebugDraw()
{
    COLORf color(0.4f, 1, 1, 1);

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
//		�_���[�W����}�l�[�W��
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


//���̃_���[�W������Ƃ�
void DamageManager::HitCheckSphere(
	const ShpereParam&	sp,
	HitEventBase&		HitEvent
	)
{
	for (auto it = m_DamageBaseMap.begin();
		it != m_DamageBaseMap.end();
		++it
		)
	{
		//�����������Ă�����
		if (it->second->HitCheckSphere(&sp) &&
			HitEvent.Hit(it->second))
		{
			//���������񐔃J�E���g�𑫂�
			++it->second->HitCount;
		}
	}
}

//�����蔻����f�o�b�O�`��
void DamageManager::DebugDraw()
{
    for (auto it = m_DamageBaseMap.begin();
         it != m_DamageBaseMap.end();
         ++it
         )
    {
        it->first->DebugDraw();
    }
}

DamageManager::DamageManager()
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