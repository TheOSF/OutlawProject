#ifndef __BASEBALL_SPATK_BALL_H__
#define __BASEBALL_SPATK_BALL_H__

#include "../utillity/LocusHDR.h"
#include "../GameSystem/GameObject.h"
#include "../GameSystem/ForwardDecl.h"
#include "../Damage/Damage.h"
#include "../character/Baseball/BaseballPlayer.h"
#include "../Render/LightObject.h"

//*****************************************************
//		通常玉クラス
//*****************************************************

class Baseball_SpAtk_Ball :public GameObjectBase
{
public:

	//コンストラクタ
	Baseball_SpAtk_Ball(
        BaseballPlayer*     pParent,
        Vector3             pos,
        Vector3             move,
		float				damage_val		//ダメージ量
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

	void UpdateDamageClass();  //ダメージ判定の位置を現在の位置に更新
	
	bool UpdateWallCheck();    //壁との判定を取り、接触していたなら移動値を反射してステート移行をする
};

#endif