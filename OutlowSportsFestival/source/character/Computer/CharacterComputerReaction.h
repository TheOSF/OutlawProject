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
        virtual void Reaction(const TypeParam& param, CrVector3 vec) = 0; //攻撃開始時に呼ばれる関数
	};

    struct InParam
    {
        float CounterAreaSize;
        float CanNearAtkAreaSize;
    };

	CharacterComputerReaction(
		CharacterBase*      pParent,	//操るキャラクタのポインタ
        InParam             param,
		ActionEvent*	    ActionEvent	
		);

	~CharacterComputerReaction();

	void Update();
    bool Reactioned()const;

private:
 
    struct StrongParam
    {
        RATIO CounterSuccess;           //カウンター検知　　　成功率
        float CounterReactionSpeed;     //カウンター検知　　　反射の速度

        RATIO NearEnemySuccess;         //敵が近くにいる検知　成功率
        float NearEnemyReactionSpeed;   //敵が近くにいる検知  反射の速度

        RATIO DamageEscapeSuccess;      //ダメージ判定の検知　成功率
        float DamageEscapeReactionSpeed;//ダメージ判定の検知  反射の速度
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
