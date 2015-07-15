#ifndef ___GAMEOBJECT_COLLISION_MESH_H__
#define ___GAMEOBJECT_COLLISION_MESH_H__

#include "../GameSystem/GameObject.h"
#include "MeshCollider.h"

//--------------------------------------------------------------
//        メッシュ判定クラスを包含したゲームオブジェクト
//--------------------------------------------------------------

class GameObjectCollisionMesh :public GameObjectBase
{
public:
    typedef MeshCollider::HitEvent HitEvent;

    GameObjectCollisionMesh(
        LPIEXMESH		pMesh,		//判定をとるメッシュ（デストラクタでdeleteしない
        HitEvent*		pHitEvent	//衝突時のイベントクラス（デストラクタでdeleteする
        );

    ~GameObjectCollisionMesh();

    //ワールド変換行列をセット
    void SetWorldMatrix(const Matrix& WorldMat);

private:
    MeshCollider m_MeshCollider;

    bool Update();
    bool Msg(MsgType mt);

};

#endif