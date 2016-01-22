#include "DamageControll_Transform.h"
#include "../character/CharacterFunction.h"

DamageControll_Transform::DamageControll_Transform() :
m_Destroy(false),
m_AddSkillGaugeValue(2),
m_pParent(nullptr)
{
    m_ChrControllArray.fill(nullptr);
    m_ChrArray.fill(nullptr);
    D3DXMatrixIdentity(&m_Transform);
}

DamageControll_Transform::~DamageControll_Transform()
{
    
}

//�t���Ă���L�����N�^�����ׂĕ���(�f�X�g���N�^�Ŏ����I�ɌĂ΂��
void DamageControll_Transform::AllFree()
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->ToFree();
        }
    }

    m_ChrControllArray.fill(nullptr);
}

void DamageControll_Transform::AllFree(CrVector3 Vec)
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->ToFree(Vec);
        }
    }
    m_ChrControllArray.fill(nullptr);
}

//���삷��L�����N�^��ǉ�����
void DamageControll_Transform::AddControllClass(CharacterDamageControll* pControllClass)
{
    MyAssert(pControllClass != nullptr, "nullptr�������܂����I");

    //�󂫂�T��
    for (int i = 0; i < (int)m_ChrControllArray.size(); ++i)
    {
        if (m_ChrControllArray.at(i) == nullptr)
        {
            m_ChrControllArray.at(i) = pControllClass;
            m_ChrArray.at(i) = pControllClass->m_pEventClass->pOwner;
            return;
        }
    }

    MyAssert(false, "�󂫌������s!");
    pControllClass->ToFree();

    
}

//�t���Ă��邷�ׂẴL�����N�^�Ƀ_���[�W��^����(�����F�P�@�_���[�W�ʁ@�F�Q�@�̗͂��P�c�����ǂ���)
void DamageControll_Transform::AddDamage(float value, bool dontDie)
{
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->AddDamage(value, dontDie);

            if (m_pParent != nullptr)
            {
                chr_func::AddSkillGauge(m_pParent, value*0.01f);
            }
        }
    }

    
}

bool DamageControll_Transform::Update()
{
    //�p���s����Z�b�g
    for (auto& it : m_ChrControllArray)
    {
        if (it != nullptr)
        {
            it->SetTransform(m_Transform);
        }
    }

    //�����t���[���Ȃ�
    if (m_Destroy)
    {
        //�t���Ă���L�����N�^�����ׂĕ���
        AllFree();
    }

    return !m_Destroy;
}

bool DamageControll_Transform::Msg(MsgType mt)
{

    return false;
}

