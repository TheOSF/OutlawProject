#pragma once

#include "CharacterBase.h"

//--------------------------------------------------------------------
//		�L�����N�^�[���� �p������_���[�W���󂯂��Ƃ��̍X�V�N���X
//--------------------------------------------------------------------

class CharacterDamageControll
{
public:
    //����N���X
    class EventClass
    {
    public:
        CharacterBase* const pOwner;            //����L�����N�^�ւ̃|�C���^
        int            const VanishMotion;      //������у��[�V�����̔ԍ�
        CharacterRenderer* const pRenderer;     //�L�����N�^�����_���[

        EventClass(CharacterBase*  pOwner, int VanishMotion, CharacterRenderer* pRenderer);

        virtual ~EventClass();
        virtual void SetVanishState(CrVector3 v) = 0;   //������уX�e�[�g�Ɉڍs����
    };

    CharacterDamageControll(EventClass* pEventClass);
    ~CharacterDamageControll();

    //�p���s����Z�b�g
    void SetTransform(const Matrix& m);

    //�������(�ʏ�̐�����уX�e�[�g�Ɉڍs����)
    void ToFree();
    void ToFree(CrVector3 Vec);

    //�_���[�W��^����(�����F�P�@�_���[�W�ʁ@�F�Q�@�̗͂��P�c�����ǂ���)
    void AddDamage(float value, bool dontDie);

    //���t���[���̍X�V
    void Update();

public:
    EventClass* const       m_pEventClass;
    Matrix                  m_Transform;

    Vector3                 m_Move;
    bool                    m_Init;

    RATIO                   m_DamageShockCount;

    int                     m_NotControllCount;
};