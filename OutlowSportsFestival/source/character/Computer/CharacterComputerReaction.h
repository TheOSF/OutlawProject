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
	//CharacterComputerMove::Param m_params;
	class ActionEvent
	{
	public:
		virtual~ActionEvent() {}
		virtual void Reaction(CharacterComputerReactionHitEvent::HitType hittype,Vector3 vec) = 0;			//攻撃開始時に呼ばれる関数
	};

	CharacterComputerReaction(
		CharacterBase*					    pParent,	//操るキャラクタのポインタ
		const CharacterComputerMove::Param&	param,		
		ActionEvent*						ActionEvent	
		);

	~CharacterComputerReaction();

	void Update();

private:

	CharacterBase*	                  m_pCharacter;
	ActionEvent*  	                  m_pActionEvent;
	CharacterComputerMove::Param	  m_Params;

    CharacterComputerReactionHitEvent::HitType m_HitType;
    Vector3                                    m_DamageVec;

    int                               m_DoActionCount;

    bool CheckBall();
    bool CheckNearCharacter();
    bool CheckDamage();
};
