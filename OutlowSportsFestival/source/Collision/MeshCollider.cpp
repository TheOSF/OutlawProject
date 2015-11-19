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
	LPVECTOR3	pOut,		//���C�����������ꍇ���̏ꏊ���i�[����
	LPVECTOR3	pPos,		//���C��ł��n�߂�ꏊ
	LPVECTOR3	pVec,		//���C��ł���
	float*		pDist,		//���C�̒���
	int*		pMaterial,	//���C�����������ꍇ���̃|���S���̍ގ��ԍ����i�[����
	RayType		type		//���C�̃^�C�v(�����𕪂���\�������邽��)
	)
{
    //���C���[����
    if ((type&m_HitFlags) == 0)
    {
        return false;
    }


    //���[���h�ϊ��s�񂪕ύX����Ă����ꍇ�A�t�s����X�V����
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
    m_ChangeMatrix = true;  //�s��ύX�t���O��true��
}