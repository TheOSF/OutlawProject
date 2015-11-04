#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/MeshRenderer.h"
#include "../Library/Bullet/BulletSystem.h"
#include "../Render/LightObject.h"
#include "../Damage/Damage.h"

//----------------------------------------------------//
//  ���H�𑖂�o�X�@�Q�[���I�u�W�F�N�g
//----------------------------------------------------//

class StageObjectBus :public GameObjectBase
{
public:

    StageObjectBus(
        CrVector3 pos,   //�o���ʒu
        CrVector3 vec,   //�o�������Ƃ��̌���
        CrVector3 goal_pos,   //���B�ʒu
        CrVector3 goal_vec,   //���B�����Ƃ��̌���
        float     Speed, //���x
        bool      light  //�w�b�h���C�g(on off)
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