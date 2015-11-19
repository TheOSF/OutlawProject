#ifndef __BASEBALL_SPATK_BALL_H__
#define __BASEBALL_SPATK_BALL_H__

#include "../utillity/LocusHDR.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"
#include "../character/Baseball/BaseballPlayer.h"
#include "../Render/LightObject.h"

//*****************************************************
//		�ʏ�ʃN���X
//*****************************************************

class Baseball_SpAtk_Ball :public GameObjectBase
{
public:

	//�R���X�g���N�^
	Baseball_SpAtk_Ball(
        BaseballPlayer*     pParent,
        Vector3             pos,
        Vector3             move,
		float				damage_val		//�_���[�W��
		);

	~Baseball_SpAtk_Ball();

	bool Update();
	bool Msg(MsgType mt);

private:
    LocusHDR            m_Locus;
	DamageCapsure		m_Damage;
	int                 m_DeleteFrame;
    BaseballPlayer*     m_pParent;
    Vector3             m_Pos, m_Vec;
    bool                m_HitWall;
    PointLight          m_PointLight;

	void UpdateDamageClass();  //�_���[�W����̈ʒu�����݂̈ʒu�ɍX�V
	
	bool UpdateWallCheck();    //�ǂƂ̔�������A�ڐG���Ă����Ȃ�ړ��l�𔽎˂��ăX�e�[�g�ڍs������
};

#endif