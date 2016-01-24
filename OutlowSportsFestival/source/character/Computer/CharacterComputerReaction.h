#pragma once
#include "../CharacterBase.h"
#include "../CharacterRenderer.h"
#include "../CharacterStateTemplate.h"

#include "CharacterComputerReactionHitEvent.h"
#include "CharacterComputerMove.h"
#include "../../Damage/Damage.h"

class CharacterComputerReaction
{
public:

	class ActionEvent
	{
	public:
        struct TypeParam
        {
            bool isEnemyNear;
            bool isStrongDamage;
            bool isBall;
            bool isCanCounter;
            bool isWeakDamage;
        };

		virtual~ActionEvent() {}
        virtual void Reaction(const TypeParam& param, CrVector3 vec) = 0; //�U���J�n���ɌĂ΂��֐�
	};

    struct InParam
    {
        float CounterAreaSize;
        float CanNearAtkAreaSize;
    };

	CharacterComputerReaction(
		CharacterBase*      pParent,	//����L�����N�^�̃|�C���^
        InParam             param,
		ActionEvent*	    ActionEvent	
		);

	~CharacterComputerReaction();

	void Update();
    bool Reactioned()const;

private:
 
    struct StrongParam
    {
        RATIO CounterSuccess;           //�J�E���^�[���m�@�@�@������
        float CounterReactionSpeed;     //�J�E���^�[���m�@�@�@���˂̑��x

        RATIO NearEnemySuccess;         //�G���߂��ɂ��錟�m�@������
        float NearEnemyReactionSpeed;   //�G���߂��ɂ��錟�m  ���˂̑��x

        RATIO DamageEscapeSuccess;      //�_���[�W����̌��m�@������
        float DamageEscapeReactionSpeed;//�_���[�W����̌��m  ���˂̑��x
    };

	CharacterBase*	const   m_pChr;
	ActionEvent*  	        m_pActionEvent;
    const InParam           m_InParam;

    ActionEvent::TypeParam  m_ReactionTypeParam;
    Vector3                 m_DamageVec;

    int                     m_DoActionCount;
    bool                    m_Reactioned;

    int                     m_DontCheckCounter;
    int                     m_DontCheckNearEnemy;
    int                     m_DontCheckDamageEscape;

    StrongParam GetParam();

    bool CheckBall();
    bool CheckNearCharacter();
    bool CheckDamage();
};
