#include "GameObjectCollisionMesh.h"

GameObjectCollisionMesh::GameObjectCollisionMesh(
    LPIEXMESH		pMesh,		
    HitEvent*		pHitEvent	
    ):
    m_MeshCollider(pMesh, pHitEvent)
{

}

GameObjectCollisionMesh::~GameObjectCollisionMesh()
{

}

//���[���h�ϊ��s����Z�b�g
void GameObjectCollisionMesh::SetWorldMatrix(const Matrix& WorldMat)
{
    m_MeshCollider.SetWorldMatrix(WorldMat);
}

bool GameObjectCollisionMesh::Update()
{
    return true;
}

bool GameObjectCollisionMesh::Msg(MsgType mt)
{
    return false;
}