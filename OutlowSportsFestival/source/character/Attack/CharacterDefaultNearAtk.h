#ifndef __CHARACTER_ATTACK_CLASS_H__
#define __CHARACTER_ATTACK_CLASS_H__

class CharacterBase;

#include "iextreme.h"
#include "Damage/Damage.h"

//****************************************************************
//		�L�����N�^���ʂ̍U���N���X
//****************************************************************


//�ʏ�U���N���X
class CharacterDefaultNearAttack
{
public:
    struct Params
    {
        int   EndFrame;          //�U���I���t���[��

        Vector3 DamagePosOffset; //�����蔻��̈ʒu(�L�����N�^��ԏ�)

        int   DamageStartFrame;  //�����蔻��o���t���[��
        int   DamageEndFrame;    //�����蔻����Ńt���[��
    };

    class AttackEvent
    {
    public:
        virtual~AttackEvent(){}

        virtual void Start() = 0; //�N���X�J�n���ɌĂ΂��
        virtual void End() = 0;   //�N���X�I�����ɌĂ΂��

        virtual void Update(int frame) = 0;	//���t���[�����s�����֐�(�����F���݂̃t���[��)

        virtual void DamageStart() = 0;			//�����蔻��o���t���[���ɌĂ΂��֐�
        virtual void DamageUpdate() = 0;        //�����蔻�肪�o�����Ă���ԂɌĂ΂��֐�
        virtual void DamageEnd() = 0;			//�����蔻����Ńt���[���ɌĂ΂��֐�
    };

    DamageShpere	m_Damage;

    CharacterDefaultNearAttack(
        CharacterBase*	pParent,	//����L�����N�^�̃|�C���^
        const Params&	param,
        AttackEvent*	pAtttackEvent
        );

    ~CharacterDefaultNearAttack();

    void Update();	//�X�V

private:
    CharacterBase*	m_pCharacter;
    AttackEvent*	m_pAttackEvent;
    Params			m_Params;
    int             m_timer;

    void UpdateDamagePos();
};

#endif