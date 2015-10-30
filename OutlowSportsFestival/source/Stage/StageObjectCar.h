#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/LightObject.h"
#include "../Damage/Damage.h"

//----------------------------------------------------//
//  道路を走る車　ゲームオブジェクト
//----------------------------------------------------//

class StageObjectCar :public GameObjectBase
{
public:
    enum CarType
    {
        Car1,
    };

    StageObjectCar(
        CrVector3 pos,   //出現位置
        CrVector3 goal,  //移動ターゲット
        float     Speed, //速度
        CarType   type,  //車タイプ
        bool      light  //ヘッドライト(on off)
        );

    ~StageObjectCar();

private:
    MeshRenderer*     m_pCarMesh;
    RigidBody*        m_pRigidBody;
    const float       m_Speed;
    const Vector3     m_GoalPos;
    RADIAN            m_Angle;
    DamageCapsure     m_Damage;

    SpotLight         m_Light;

    bool              m_LiveFlg;

    bool Update();
    bool Msg(MsgType mt);

    void  Move();
    void  UpdateRigidBody();
    void  UpdateDamage();
    bool  isGoal();
    float GetCarScale(CarType type);
    void  LightPosUpdate();
};