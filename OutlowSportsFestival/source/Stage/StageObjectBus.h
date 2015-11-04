#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/LightObject.h"
#include "../Damage/Damage.h"

//----------------------------------------------------//
//  道路を走るバス　ゲームオブジェクト
//----------------------------------------------------//

class StageObjectBus :public GameObjectBase
{
public:

    StageObjectBus(
        CrVector3 pos,   //出現位置
        CrVector3 vec,   //出現したときの向き
        CrVector3 goal_pos,   //到達位置
        CrVector3 goal_vec,   //到達したときの向き
        float     Speed, //速度
        bool      light  //ヘッドライト(on off)
        );

    ~StageObjectBus();

private:
    MeshRenderer*     m_pMesh;
    RigidBody*        m_pRigidBody;
    const float       m_TargetSpeed;
    float             m_MoveSpeed;
    const Vector3     m_GoalPos;
    RADIAN            m_Angle;
    DamageCapsure     m_Damage;

    SpotLight         m_Light;

    bool              m_LiveFlg;

    bool Update();
    bool Msg(MsgType mt);

    void  AttackSucces();
    void  Move();
    void  UpdateRigidBody();
    void  UpdateDamage();
    bool  isGoal();
    void  LightPosUpdate();
};