#include "StageObjectFactory.h"
#include "StageDamagePhysicMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "../Effect/HitEffectObject.h"
#include "../debug/DebugDraw.h"

//” ì¬
class BoxCreater :public StageDamagePhysicMoveObject::RigidBodyCreater
{
public:
    float mass;
    Vector3 pos, angle, scale;
    float friction;
    float restitution;

    RigidBody* Create(CrVector3 v)
    {
        RigidBody* ret = DefBulletSystem.AddRigidBox(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos,angle,scale,friction,restitution,
            v
            );

        return ret;
    }
};


class CapsureCreater :public StageDamagePhysicMoveObject::RigidBodyCreater
{
public:
    float mass;
    Vector3 pos, angle;
    float radius;
    float height;
    float friction;
    float restitution;

    RigidBody* Create(CrVector3 v)
    {
        RigidBody* ret = DefBulletSystem.AddRigidCapsure(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos, angle, radius, height, friction, restitution,
            v
            );

        return ret;
    }
};


class ConeCreater :public StageDamagePhysicMoveObject::RigidBodyCreater
{
public:
    float mass;
    Vector3 pos, angle;
    float radius;
    float height;
    float friction;
    float restitution;

    RigidBody* Create(CrVector3 v)
    {
        RigidBody* ret = DefBulletSystem.AddRigidCone(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos, angle, radius, height, friction, restitution,
            v*70.0f
            );

        new HitEffectObject(
            pos,
            v,
            0.025f,
            0.075f,
            Vector3(1,1,1)
            );

        return ret;
    }
};




//-----------------------------------------------------------------//

void StageObjFactory::CreateCone(CrVector3 pos, CrVector3 angle)
{
    ConeCreater* c = new ConeCreater();

    c->angle = angle;
    c->friction = 1000.0f;
    c->height = 3.0f;
    c->mass = 5.0f;
    c->pos = pos;
    c->radius = 1.25f;
    c->restitution = 0.2f;

    MeshCollider* pCollider =
        new MeshCollider(
        DefResource.Get(Resource::MeshType::Sphere),
        new MeshCollider::HitEvent()
        );

    {
        Matrix m;

        D3DXMatrixScaling(&m, 0.6f, 0.6f, 0.6f);

        m._41 = pos.x;
        m._42 = pos.y - 0.5f;
        m._43 = pos.z;

        pCollider->SetWorldMatrix(
            m
            );

        //new DebugDrawSphere(
        //    Vector3(m._41, m._42, m._43),
        //    2,
        //    COLORf(1, 1, 0, 0.5f),
        //    600
        //    );

    }

    new StageDamagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Cone), false, MeshRenderer::RenderType::UseColorSpecular),
        pos,
        Vector3(1, 1, 1)*0.05f,
        angle,
        1.5f,
        c,
        pCollider,
        Vector3(0, -1.3f, 0)
        );
}

void StageObjFactory::CreatePipe(CrVector3 pos, CrVector3 angle)
{
    CapsureCreater* c = new CapsureCreater();

    c->angle = angle;
    c->friction = 0.5f;
    c->height = 1.0f;
    c->mass = 5.0f;
    c->pos = pos;
    c->radius = 0.05f;
    c->restitution = 0.5f;


    MeshCollider* pCollider =
        new MeshCollider(
        DefResource.Get(Resource::MeshType::Pole),
        new MeshCollider::HitEvent()
        );

    {
        Matrix m;

        D3DXMatrixScaling(&m, 1, 1, 1);

        m._41 = pos.x;
        m._42 = pos.y;
        m._43 = pos.z;

        pCollider->SetWorldMatrix(
            m
            );

    }

    new StageDamagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Pipe), false, MeshRenderer::RenderType::UseColorSpecular),
        pos,
        Vector3(1, 1, 1)*0.05f,
        angle,
        3,
        c,
        pCollider
        );
}

void StageObjFactory::CreateBench(CrVector3 pos, CrVector3 angle)
{
    BoxCreater* c = new BoxCreater();

    c->angle = angle;
    c->friction = 0.5f;
    c->scale = Vector3(4, 0.3f, 0.5f);
    c->mass = 15.0f;
    c->pos = pos;
    c->restitution = 0.5f;


    MeshCollider* pCollider =
        new MeshCollider(
        DefResource.Get(Resource::MeshType::Pole),
        new MeshCollider::HitEvent()
        );

    {
        Matrix m;

        D3DXMatrixScaling(&m, 1, 1, 1);

        m._41 = pos.x;
        m._42 = pos.y;
        m._43 = pos.z;

        pCollider->SetWorldMatrix(
            m
            );

    }

    new StageDamagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Bench), false, MeshRenderer::RenderType::UseColorSpecular),
        pos,
        Vector3(1, 1, 1)*0.05f,
        angle,
        3,
        c,
        pCollider
        );
}