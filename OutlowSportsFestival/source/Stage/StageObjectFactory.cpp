#include "StageObjectFactory.h"
#include "StagePhysicMoveObject.h"
#include "../GameSystem/ResourceManager.h"
#include "../debug/DebugDraw.h"

//î†çÏê¨
class BoxCreater
{
public:
    float mass;
    Vector3 pos, angle, scale, Inertia;
    float friction;
    float restitution;

    RigidBody* Create()
    {
        RigidBody* ret = DefBulletSystem.AddRigidBox(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos,angle,scale,friction,restitution,
            Vector3Zero,
            Inertia
            );

        return ret;
    }
};


class CapsureCreater
{
public:
    float mass;
    Vector3 pos, angle;
    float radius;
    float height;
    float friction;
    float restitution;

    RigidBody* Create()
    {
        RigidBody* ret = DefBulletSystem.AddRigidCapsure(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos, angle, radius, height, friction, restitution,
            Vector3Zero
            );

        return ret;
    }
};


class ConeCreater
{
public:
    float mass;
    Vector3 pos, angle, Inertia;
    float radius;
    float height;
    float friction;
    float restitution;

    RigidBody* Create()
    {
        RigidBody* ret = DefBulletSystem.AddRigidCone(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos, angle, radius, height, friction, restitution,
            Vector3Zero
            );
        
        return ret;
    }
};




//-----------------------------------------------------------------//

void StageObjFactory::CreateCone(CrVector3 pos, CrVector3 angle)
{
    ConeCreater c;

    c.angle = angle;
    c.friction = 5.0f;
    c.height = 3.8f;
    c.mass = 5.0f;
    c.pos = pos;
    c.radius = 1.25f;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, -1.5f, 0);


    new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Cone), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(1, 1, 1)*0.05f,
        2.0f
        );
}

void StageObjFactory::CreatePipe(CrVector3 pos, CrVector3 angle)
{
    BoxCreater c;

    c.angle = angle;
    c.friction = 5.0f;
    c.scale = Vector3(0.35f, 5.5f, 0.35f);
    c.mass = 100.0f;
    c.pos = pos;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, 0, 0);

    new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Pipe), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(2.5f, 1, 2.5f)*0.1f,
        2.0f
        );

}

void StageObjFactory::CreateBench(CrVector3 pos, CrVector3 angle)
{
    BoxCreater c;

     
    c.angle = angle;
    c.friction = 5.0f;
    c.scale = Vector3(4.3f, 0.65f, 0.8f);
    c.mass = 30.0f;
    c.pos = pos;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, 1, 0);


    new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Bench), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(1, 1, 1)*0.05f,
        2.0f
        );
}