#include "StageObjectFactory.h"
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

class ConeBoxCreater
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
        RigidBody* ret = DefBulletSystem.AddRigidBoxAndCone(
            mass,
            RigidBody::CollisionTypes::ct_dynamic,
            pos, angle, radius, height, friction, restitution,
            Vector3Zero
            );

        return ret;
    }
};



//-----------------------------------------------------------------//

StagePhysicMoveObject* StageObjFactory::CreateCone(CrVector3 pos, CrVector3 angle)
{
    const float sizeRatio = 0.8f;

    ConeBoxCreater c;

    c.angle = angle;
    c.friction = 5.0f;
    c.height = 3.8f*sizeRatio;
    c.mass = 5.0f;
    c.pos = pos;
    c.radius = 1.25f*sizeRatio;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, -1.5f, 0)*sizeRatio;


    return new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Cone), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(1, 1, 1)*0.05f*sizeRatio,
        2.0f*sizeRatio
        );
}

StagePhysicMoveObject* StageObjFactory::CreatePipe(CrVector3 pos, CrVector3 angle)
{
    BoxCreater c;

    c.angle = angle;
    c.friction = 5.0f;
    c.scale = Vector3(0.35f, 5.5f, 0.35f);
    c.mass = 100.0f;
    c.pos = pos;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, 0, 0);

    return new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Pipe), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(2.5f, 1, 2.5f)*0.1f,
        2.0f
        );

}

StagePhysicMoveObject* StageObjFactory::CreateBench(CrVector3 pos, CrVector3 angle)
{
    const float sizeRatio = 0.85f;

    BoxCreater c;

     
    c.angle = angle;
    c.friction = 5.0f;
    c.scale = Vector3(4.3f, 0.65f*1.3f, 0.8f)*sizeRatio;
    c.mass = 30.0f;
    c.pos = pos;
    c.restitution = 0.2f;
    c.Inertia = Vector3(0, 1, 0)*sizeRatio;


    return new StagePhysicMoveObject(
        new MeshRenderer(DefResource.Get(Resource::MeshType::Bench), false, MeshRenderer::RenderType::UseColorSpecular),
        c.Create(),
        Vector3(1, 1.3f, 1)*0.05f*sizeRatio,
        2.0f*sizeRatio
        );
}