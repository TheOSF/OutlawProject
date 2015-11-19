#include "MeshCollider.h"

MeshCollider::MeshCollider(
    LPIEXMESH		pMesh,
    HitEvent*		pHitEvent,
    bool            MeshDelete,
    DWORD           hitFlags
    ) :
    m_pMesh(pMesh),
    m_pHitEvent(pHitEvent),
    m_ChangeMatrix(false),
    m_HitFlags(hitFlags),
    m_MeshDelete(MeshDelete)
{
    D3DXMatrixIdentity(&m_TransMatrix);
    D3DXMatrixIdentity(&m_TransInvMatrix);
}

MeshCollider::~MeshCollider()
{
	delete m_pHitEvent;

    if (m_MeshDelete)
    {
        delete m_pMesh;
    }
}

bool MeshCollider::RayPick(
	LPVECTOR3	pOut,		//レイが当たった場合その場所を格納する
	LPVECTOR3	pPos,		//レイを打ち始める場所
	LPVECTOR3	pVec,		//レイを打つ方向
	float*		pDist,		//レイの長さ
	int*		pMaterial,	//レイが当たった場合そのポリゴンの材質番号を格納する
	RayType		type		//レイのタイプ(処理を分ける可能性があるため)
	)
{
    //レイヤー判定
    if ((type&m_HitFlags) == 0)
    {
        return false;
    }


    //ワールド変換行列が変更されていた場合、逆行列を更新する
    if (m_ChangeMatrix)
    {
        m_ChangeMatrix = false;
        D3DXMatrixInverse(&m_TransInvMatrix, 0, &m_TransMatrix);
    }

	*pMaterial = 0;
    
	return m_pMesh->RayPickPlus(
		pOut,
		pPos,
		pVec,
		pDist,
        m_TransMatrix,
        m_TransInvMatrix
		) >= 0;
}

void MeshCollider::Hit(
    RayType     hit_Raytype,
	CrVector3	hit_pos,
	int			hit_material
	)
{
    if (m_pHitEvent)
    {
        m_pHitEvent->Hit(hit_Raytype, hit_pos, hit_material);
    }
    
}

void MeshCollider::SetWorldMatrix(const Matrix& mat)
{
    m_TransMatrix = mat;
    m_ChangeMatrix = true;  //行列変更フラグをtrueに
}