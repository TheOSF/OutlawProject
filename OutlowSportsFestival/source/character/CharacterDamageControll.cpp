#include "CharacterDamageControll.h"
#include "CharacterFunction.h"

#include "../Sound/Sound.h"
#include "../Effect/EffectFactory.h"


CharacterDamageControll::EventClass::EventClass(CharacterBase*  pOwner, int VanishMotion):
pOwner(pOwner),
VanishMotion(VanishMotion)
{

}

CharacterDamageControll::EventClass:: ~EventClass()
{

}

//---------------------------------------------------------------//

CharacterDamageControll::CharacterDamageControll(EventClass* pEventClass) :
m_pEventClass(pEventClass),
m_Move(Vector3Zero),
m_Init(false),
m_NotControllCount(0)
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
    m_Transform = m;
    m_NotControllCount = 0;
}

//�������(�ʏ�̐�����уX�e�[�g�Ɉڍs����)
void CharacterDamageControll::ToFree()
{
    const float MaxMove = 2.0f;

    if (m_Move.Length() > MaxMove)
    {
        m_Move.Normalize();
        m_Move *= MaxMove;
    }

    m_pEventClass->SetVanishState(m_Move);
}

//�_���[�W��^����(�����F�P�@�_���[�W�ʁ@�F�Q�@�̗͂��P�c�����ǂ���)
void CharacterDamageControll::AddDamage(float value, bool dontDie)
{
    //�_���[�W�v�Z
    chr_func::CalcDamage(m_pEventClass->pOwner, value, dontDie);


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
        m_pEventClass->pOwner->m_Renderer.SetMotion(m_pEventClass->VanishMotion);
    }
    
    const float ShockValue = 0.5f;  //�_���[�W�̂�� 

    CharacterBase* const pChr = m_pEventClass->pOwner;
    Vector3 PrePos = pChr->m_Params.pos;
    Vector3 DamageShockDiffPos = Vector3Zero;

    //�A�j���[�V�����X�V
    pChr->m_Renderer.Update(1);


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

        pChr->m_Renderer.m_TransMatrix = ScaleMat;
        pChr->m_Renderer.m_TransMatrix *= m_Transform;

        {
            Matrix* p = &pChr->m_Renderer.m_TransMatrix;

            pChr->m_Params.pos = Vector3(p->_41, p->_42, p->_43);
        }

        {
            Matrix* p = &pChr->m_Renderer.m_TransMatrix;

            p->_41 += DamageShockDiffPos.x;
            p->_42 += DamageShockDiffPos.y;
            p->_43 += DamageShockDiffPos.z;
        }
    }

    //�O��t���[���̈ʒu����ړ��l���Z�o
    {
        m_Move = m_pEventClass->pOwner->m_Params.move = (pChr->m_Params.pos - PrePos);
    }

    if (m_NotControllCount++ > 60)
    {
        ToFree();
    }
}

