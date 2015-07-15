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

//ワールド変換行列をセット
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