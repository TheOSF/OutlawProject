#ifndef __MESH_COLLIDER_H__
#define __MESH_COLLIDER_H__

#include "Collision.h"


//************************************************************
//	メッシュのあたり判定クラス
//************************************************************
class MeshCollider :public CollisionBase
{
public:
	class HitEvent
	{
	public:
        virtual void Hit(
            RayType     hit_Raytype,
            CrVector3	hit_pos,
            int			hit_material){}
	};

    MeshCollider(
        LPIEXMESH		pMesh,		//デストラクタでdeleteしない
        HitEvent*		pHitEvent,	//デストラクタでdeleteする
        bool            MeshDelete = false, 
        DWORD           hitFlags = 0xFFFFFFFF
		);

	~MeshCollider();

	bool RayPick(
		LPVECTOR3	pOut,		//レイが当たった場合その場所を格納する
		LPVECTOR3	pPos,		//レイを打ち始める場所
		LPVECTOR3	pVec,		//レイを打つ方向
		float*		pDist,		//レイの長さ
		int*		pMaterial,	//レイが当たった場合そのポリゴンの材質番号を格納する
		RayType		type		//レイのタイプ(処理を分ける可能性があるため)
		);

    //当たった時の処理関数を呼び出す
	void Hit(
        RayType     hit_Raytype,
		CrVector3	hit_pos,
		int			hit_material
		);

    //ワールド変換行列をセットする
    void SetWorldMatrix(const Matrix& mat);
    
private:
	LPIEXMESH		m_pMesh;
	HitEvent*		m_pHitEvent;
    Matrix          m_TransMatrix;
    Matrix          m_TransInvMatrix;
    bool            m_ChangeMatrix;
    bool            m_MeshDelete;
    DWORD           m_HitFlags;
};

#endif