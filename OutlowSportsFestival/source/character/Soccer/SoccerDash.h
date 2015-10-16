#ifndef __CHARACTER_DASH_CLASS_H__
#define __CHARACTER_DASH_CLASS_H__

#include "iextreme.h"
#include "SoccerPlayer.h"
#include "../../Damage/Damage.h"


//****************************************************************
//		�T�b�J�[�̃_�b�V���N���X
//****************************************************************
//�X���C�f�B���O�N���X
class SoccerDash
{
public:
	SoccerDash(
		SoccerPlayer* pParent	//����L�����N�^�̃|�C���^
		);


	~SoccerDash();

	void Update();	//�X�V
	void UpdateLocusColor();
	void SetStickValue(CrVector2 StickValue);
	void DoShot();
	void DoSliding();
	void DoCounter();
	void DoAvoid();
	void DoFinisher();
	void DoStop();
private:
	typedef DamageManager::HitEventBase HitEventBase;

	SoccerPlayer*	m_pParent;
	Vector2			m_StickValue;
	Locus           m_Locus;
	bool			m_isRun;
	bool            m_Init;
};
#endif