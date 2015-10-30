#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/LightObject.h"
#include "../Damage/Damage.h"

//----------------------------------------------------//
//  ���H�𑖂�ԁ@�Q�[���I�u�W�F�N�g
//----------------------------------------------------//

class StageObjectCar :public GameObjectBase
{
public:
    enum CarType
    {
        Car1,
    };

    StageObjectCar(
        CrVector3 pos,   //�o���ʒu
        CrVector3 goal,  //�ړ��^�[�Q�b�g
        float     Speed, //���x
        CarType   type,  //�ԃ^�C�v
        bool      light  //�w�b�h���C�g(on off)
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