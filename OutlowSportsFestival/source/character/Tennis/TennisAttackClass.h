#pragma once

#include "iextreme.h"
#include "Damage/Damage.h"
#include "TennisPlayer.h"
#include "../../utillity/LocusHDR.h"
#include <vector>

//****************************************************************
//		�e�j�X�̍U���N���X(COM,PLAYER����)
//****************************************************************


class TennisAttackClass
{
public:

    //�U������N���X
    class ControllEvent
    {
    public:
        virtual ~ControllEvent(){}
        virtual bool isDoCombo() = 0;    //�߂�Ƃ���true��Ԃ��ƃR���{���s������
        virtual void AngleControll(RADIAN angle) = 0;//�A���O���ύX���s����t���[���ɌĂ΂��
    };

    //�U�����N���X
    class AttackInfo
    {
    public:

        virtual ~AttackInfo(){}

        //�_���[�W�p�����[�^���Z�b�g
        virtual void DamageParamSet(DamageShpere* pDmg) = 0;

        //�_���[�W�ʒu���Z�b�g
        virtual void DamagePosSet(DamageShpere* pDmg, TennisPlayer* pTennis) = 0;

        //���[�V�������Z�b�g
        virtual void MotionSet(TennisPlayer* pTennis) = 0;

        //�����̃t���[�����U���I�����ǂ���
        virtual bool isEnd(int Frame) = 0;

        //�����̃t���[�����R���{�ڍs�t���[�����ǂ���
        virtual bool isComboSwitchFrame(int Frame) = 0;

        //�����̃t���[�����A�R���{�̈ڍs����{�^����������t���[�����ǂ���
        virtual bool isComboButtonFrame(int Frame) = 0;

        //�����̃t���[�����A�p�x�������s����t���[�����ǂ���
        virtual bool isCanAngleControll(int Frame,RADIAN& OutControllVal) = 0;

        //�����̃t���[�����A�_���[�W����L���t���[�����ǂ���
        virtual bool isDamageEnable(int Frame) = 0;

        //���t���[���Ă΂��
        virtual void Update(int Frame, LocusHDR* pLocus) = 0;

        //�U�������������Ƃ��ɌĂ΂��
        virtual void HitAttack(DamageShpere* pDmg) = 0;
    };

    typedef std::vector<AttackInfo*> AttackInfoArray;
    
    AttackInfoArray       m_AttackInfoArray; //�U�����N���X�z��(���ꂽ�������R���{�ł���)


    TennisAttackClass(
        TennisPlayer*   pOwner,  //�e�j�X�v���C���[�ւ̃|�C���^
        ControllEvent*  pEvent   //�C�x���g�N���X(�I������delete����)
        );

    ~TennisAttackClass();

    void Update();	//�X�V

    bool isEnd()const;   //�I���m�F

  
    
private:

    TennisPlayer*  const  m_pOwner;
    ControllEvent* const  m_pEvent;
    DamageShpere	      m_Damage;
    int                   m_Timer;
    int                   m_ComboCount;
    bool                  m_DoCombo;
    LocusHDR              m_Locus;
    int                   m_DamageHitCount;

    void(TennisAttackClass::*m_pStateFunc)();

    void State_Attack();
    void State_NextAtk();
    void State_End();

    bool isLastAtk();
};
