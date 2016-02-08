#include "Damage.h"
#include "../debug/DebugDraw.h"
#include "../character/CharacterBase.h"
#include "../character/CharacterFunction.h"

//�_���[�W�N���X
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
	MyAssert(DefDamageMgr.AddDamage(this), "�_���[�W�o�^�̎��s");
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
	MyAssert(DefDamageMgr.EraceDamage(this), "�_���[�W�폜�̎��s");
#else
    DefDamageMgr.EraceDamage(this);
#endif
}

bool DamageBase::isOptionOn(Option op)const
{
    //�����̃I�v�V������on���ǂ���
    return m_OptionFlags.at((int)op);
}
void DamageBase::SetOption(Option op, bool flag)
{
    //�I�v�V������on,off���Z�b�g
    m_OptionFlags.at((int)op) = flag;
}

void DamageBase::ResetCounts()
{
    //�q�b�g�J�E���g�A�L�����q�b�g�J�E���g�����Z�b�g
    m_PlayerHitCounts.fill(0);
    HitCount = 0;
}

void DamageBase::OnHitCharacter(CharacterBase* pHitChr)
{
    //�L�����N�^�ɓ��������ꍇ�J�E���g�����Z
    ++m_PlayerHitCounts.at((int)pHitChr->m_PlayerInfo.number);

    //�X�L���Q�[�W���Z
    if (pParent != nullptr)
    {
        chr_func::AddSkillGauge(pParent, Value * AddSkillGaugeValue * 0.01f);
    }
}

bool DamageBase::isCanHitCharacter(CharacterBase* pHitChr)const
{
    //�����̃L�����N�^�ɓ�����邩�ǂ���
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

//�_���[�W�N���X(��)
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
    //�_���[�W�ʒu�͋��̒��S
    *pOutPos = m_Param.pos;

    //����
    switch (m_VecType)
    {
    case DamageVecType::MemberParam:
        //�Œ�l
        *pOutVec = m_Vec;
        break;

    case DamageVecType::CenterToPos:
    case DamageVecType::CenterToPosXZ:
        //���ˏ�
        *pOutVec = hit_pos - m_Param.pos;
        break;

    case DamageVecType::PosToCenter:
    case DamageVecType::PosToCenterXZ:
        //���S�Ɍ�������
        *pOutVec = m_Param.pos - hit_pos;
        break;

    default:
        MyAssert(false, "m_VecType���s���ł� type = %d ", (int)m_VecType);
        *pOutVec = Vector3AxisX;
        break;
    }


    //�x���ړ��̌Œ�̏ꍇ
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
        //���K��
        *pOutVec = Vector3Normalize(*pOutVec);
    }

    //�����p���[��������
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

//�_���[�W�N���X(�J�v�Z��)
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
    //�ŋߖT�_�̎Z�o
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


    //����
    {
        
        //����
        switch (m_VecType)
        {
        case DamageVecType::MemberParam:
            //�Œ�l
            *pOutVec = m_Vec;
            break;

        case DamageVecType::CenterToPos:
        case DamageVecType::CenterToPosXZ:
            //���ˏ�
            *pOutVec = hit_pos - *pOutPos;
            break;

        case DamageVecType::PosToCenter:
        case DamageVecType::PosToCenterXZ:
            //���S�Ɍ�������
            *pOutVec = *pOutPos - hit_pos;
            break;

        default:
            MyAssert(false, "m_VecType���s���ł� type = %d ", (int)m_VecType);
            *pOutVec = Vector3AxisX;
            break;
        }

        //�x���ړ��̌Œ�̏ꍇ
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
            //���K��
            *pOutVec = Vector3Normalize(*pOutVec);
        }

        //�����p���[��������
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
//	�p������_���[�W�N���X(�`��͋�)
//*************************************************************


//�R���X�g���N�^
DamageControllVanish::DamageControllVanish(
    GetDamageControllTransformClass*     pGetDamageControllClass  //�f�X�g���N�^��delete����
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
    const SphereParam&	sp,
	HitEventBase&		HitEvent
	)
{

    if (m_DebugDrawVisible)
    {
        //�f�o�b�O�p���`��(��)
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
		//�����������Ă�����
		if (it->second->HitCheckSphere(&sp) &&
			HitEvent.Hit(it->second))
		{
			//���������񐔃J�E���g�𑫂�
			++it->second->HitCount;
            
            //�R�[���o�b�N�֐��Ăяo��
            if (it->second->pCallBackClass != nullptr)
            {
                it->second->pCallBackClass->Hit(&sp);
            }
            break;
		}
	}
}


//�����蔻����f�o�b�O�`��
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