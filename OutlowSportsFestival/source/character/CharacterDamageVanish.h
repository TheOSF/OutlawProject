#pragma once

#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"

//***************************************************
//		�L�����N�^�[���� ������тЂ�݃N���X
//***************************************************
class CharacterDamageVanish
{
public:
	struct Param
	{
		Vector3 move;         //�ړ����x
		Vector3 rotate_speed; //�e���̉�]���x

        UINT    standup_frame;//���n���Ă��瑀��\�ɂȂ�܂ł̎���
	};

    class Event
    {
    public:
        virtual ~Event() {}
        virtual void Flying(const Matrix& Rotate) = 0;//���ł���Œ��ɌĂ΂��֐�
        virtual void StandUpStart() = 0;              //�����オ��X�^�[�g���ɌĂ΂��֐�
        virtual void StandUping() = 0;                //�����オ���Ă���Œ��ɌĂ΂��֐�
        virtual void Start() = 0;                     //������уX�^�[�g���ɌĂ΂��֐�
        virtual void End() = 0;                       //�����オ��I�����ɌĂ΂��֐�
    };

    CharacterDamageVanish(
        CharacterBase*    pCharacter,//������ԃL�����N�^
        const Param&      param, //�����Ƃуp�����[�^�[
        Event*            pEvent //�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
        );

    ~CharacterDamageVanish();

    // �X�V
    void Update();

private:

    CharacterBase*  m_pCharacter;
    Param           m_Param;
    Vector3         m_Rotate;
    bool            m_Start;
    bool            m_End;
    Event*          m_pEvent;      // �C�x���g
    UINT            m_Count;
    void(CharacterDamageVanish::*m_pStateFunc)();


    void Flying();
    void StandUp();
};

