#ifndef __HIT_STAGE_OBJECT_H__
#define __HIT_STAGE_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../Collision/MeshCollider.h"
#include "..//Render/MeshRenderer.h"


//---------------------------------------------------
//  当たり判定のあるステージオブジェクトクラス
//---------------------------------------------------

class HitStageObject :public GameObjectBase
{
public:

    HitStageObject(
        MeshRenderer*    pMeshRenderer,  //メッシュ描画クラス
        MeshCollider*   pMeshCollider   //メッシュあたり判定クラス
        );

    ~HitStageObject();

    void SetMatrix(const Matrix& mat);
private:

    MeshRenderer*  const m_pMeshRenderer;  //メッシュ描画クラス
    MeshCollider* const m_pMeshCollider;   //メッシュあたり判定クラス

    bool Update();
    bool Msg(MsgType mt);
};

#endif