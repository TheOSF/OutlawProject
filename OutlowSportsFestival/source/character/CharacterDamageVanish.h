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

        int     down_frame;
        int     down_muteki_frame;

        int     standup_frame;
        int     standup_muteki_frame;
	};

    class Event
    {
    public:
        virtual ~Event() {}
        virtual void FlyStart() = 0;                     //������уX�^�[�g
        virtual void Flying(const Matrix& Rotate) = 0;   //�������ł���Œ�
        virtual void DownStart() = 0;                         //���n��
        virtual void Downing() = 0;                      //�������낪���Ă���Œ�
        virtual void StandUpStart() = 0;                 //�N���オ��X�^�[�g
        virtual void StandUping() = 0;                  //�N���オ��Œ�
        virtual void End() = 0;                          //�X�e�[�g�I��
    };

    CharacterDamageVanish(
        CharacterBase*    pCharacter,//������ԃL�����N�^
        const Param&      param, //�����Ƃуp�����[�^�[
        Event*            pEvent, //�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
        DamageManager::HitEventBase* pHitEvent //�q�b�g�C�x���g�N���X�ւ̃|�C���^(�f�X�g���N�^��delete����)
        );

    ~CharacterDamageVanish();

    // �X�V
    void Update();

private:
    DamageManager::HitEventBase* m_pHitEvent;
    CharacterBase*  m_pCharacter;
    Param           m_Param;
    Vector3         m_Rotate;
    Event*          m_pEvent;      // �C�x���g
    int             m_Count;

    void(CharacterDamageVanish::*m_pStateFunc)();

    void Initialize();
    void Flying();
    void Dowing();
    void StandUping();
    void End();
};

