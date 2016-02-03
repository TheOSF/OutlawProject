#include "CharacterDamageControll.h"
#include "CharacterFunction.h"

#include "../Sound/Sound.h"
#include "../Effect/EffectFactory.h"

#include "CharacterHitEventFunc.h"

CharacterDamageControll::EventClass::EventClass(CharacterBase*  pOwner, int VanishMotion, CharacterRenderer* pRenderer) :
pOwner(pOwner),
VanishMotion(VanishMotion),
pRenderer(pRenderer),
PosOffset(0, 0, 0)
{

}

CharacterDamageControll::EventClass::~EventClass()
{

}

//---------------------------------------------------------------//

CharacterDamageControll::CharacterDamageControll(EventClass* pEventClass) :
m_pEventClass(pEventClass),
m_Move(Vector3Zero),
m_Init(false),
m_NotControllCount(0),
m_DamageShockCount(0)
{
    D3DXMatrixIdentity(&m_Transform);

    MyAssert(m_pEventClass != nullptr, "�C�x���g�N���X��nullptr�ɂȂ��Ă��܂�");

    m_pEventClass->pOwner->m_Params.DoCheckOtherChrSpace = false;
}

CharacterDamageControll::~CharacterDamageControll()
{
    m_pEventClass->pOwner->m_Params.DoCheckOtherChrSpace = true;
    delete m_pEventClass;
}

//�p���s����Z�b�g
void CharacterDamageControll::SetTransform(const Matrix& m)
{

    Matrix M;
    D3DXMatrixTranslation(&M, m_pEventClass->PosOffset.x, m_pEventClass->PosOffset.y, m_pEventClass->PosOffset.z);

    m_Transform = M*m;
    m_NotControllCount = 0;
}

//�������(�ʏ�̐�����уX�e�[�g�Ɉڍs����)
void CharacterDamageControll::ToFree()
{
    const float MaxMove = 0.6f;

    if (m_Move.Length() > MaxMove)
    {
        m_Move.Normalize();
        m_Move *= MaxMove;
    }

    ToFree(m_Move);
}

void CharacterDamageControll::ToFree(CrVector3 Vec)
{
    m_pEventClass->SetVanishState(Vec);
}

//�_���[�W��^����(�����F�P�@�_���[�W�ʁ@�F�Q�@�̗͂��P�c�����ǂ���)
void CharacterDamageControll::AddDamage(float value, bool dontDie)
{
    //�_���[�W�v�Z
    chr_func::CalcDamage(m_pEventClass->pOwner, value, dontDie);

    //�󂯂��_���[�W�l�ɂ���ăQ�[�W����
    chr_func::AddSkillGauge(m_pEventClass->pOwner, value * CharacterHitEventFunc::m_DamageAddSkillValue);


    EffectFactory::HitEffect(
        m_pEventClass->pOwner,
        Vector3Normalize(m_Move)
        );

    //�r�d
    Sound::Play(Sound::Damage3);

    //�ʒu�̃Y���J�E���g���@�Z�b�g
    m_DamageShockCount = 1.0f;
}



//���t���[���̍X�V
void CharacterDamageControll::Update()
{
    //�����t���[���Ń��[�V�����Z�b�g
    if (m_Init == false)
    {
        m_Init = true;
        m_pEventClass->pRenderer->SetMotion(m_pEventClass->VanishMotion);
    }
    
    const float ShockValue = 0.5f;  //�_���[�W�̂�� 

    CharacterBase* const pChr = m_pEventClass->pOwner;
    Vector3 PrePos = pChr->m_Params.pos;
    Vector3 DamageShockDiffPos = Vector3Zero;

    //�A�j���[�V�����X�V
    m_pEventClass->pRenderer->Update(1);


    //�_���[�W�ɂ��U���̈ʒu���X�V
    if (m_DamageShockCount > 0)
    {
        DamageShockDiffPos = Vector3Rand()*m_DamageShockCount*ShockValue;
        m_DamageShockCount -= 0.08f;
    }

    //���[���h�ϊ��s����쐬�����
    {
        Matrix ScaleMat;

        D3DXMatrixScaling(&ScaleMat, pChr->m_ModelSize, pChr->m_ModelSize, pChr->m_ModelSize);

        m_pEventClass->pRenderer->m_TransMatrix = ScaleMat;
        m_pEventClass->pRenderer->m_TransMatrix *= m_Transform;

        {
            Matrix* p = &m_pEventClass->pRenderer->m_TransMatrix;

            pChr->m_Params.pos = Vector3(p->_41, p->_42, p->_43);
        }

        {
            Matrix* p = &m_pEventClass->pRenderer->m_TransMatrix;

            p->_41 += DamageShockDiffPos.x;
            p->_42 += DamageShockDiffPos.y;
            p->_43 += DamageShockDiffPos.z;
        }
    }

    //�O��t���[���̈ʒu����ړ��l���Z�o
    {
        m_Move = m_pEventClass->pOwner->m_Params.move = (pChr->m_Params.pos - PrePos);
    }

    if (m_NotControllCount++ > 5)
    {
        ToFree();
    }
}

