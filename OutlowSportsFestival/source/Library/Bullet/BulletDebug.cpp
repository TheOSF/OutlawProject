#include "BulletDebug.h"
#include "DebugDrawManager.h"
#include "../../debug/DebugDraw.h"

//*****************************************************************
//          BulletDebug�N���X
//          DebugDrawManager�N���X ���g�p���ĕ`�悵�܂��B
//*****************************************************************


// �R���X�g���N�^
BulletDebugDrawer::BulletDebugDrawer(IDirect3DDevice9* pDevice) :
m_debugDrawMode(btIDebugDraw::DBG_NoDebug),
m_pDevice(pDevice),
m_Vb(pDevice, sizeof(D3DXVECTOR3)* 2)
{



}


// �f�X�g���N�^
BulletDebugDrawer::~BulletDebugDrawer()
{
	m_pDevice = nullptr;
}


// btVector3��D3DXVECTOR3
const D3DXVECTOR3 BulletDebugDrawer::btVector3ToVector3(const btVector3& btVec3)
{
	D3DXVECTOR3 vec;
	vec.x = btVec3.x();
	vec.y = btVec3.y();
	vec.z = btVec3.z();
	return vec;
}


// btVector3��Vector3
Vector3 btVector3ToIexVector3(const btVector3& btVec3)
{
    Vector3 vec;
    vec.x = btVec3.x();
    vec.y = btVec3.y();
    vec.z = btVec3.z();
    return vec;
}


// btVector3��D3DXCOLOR
const D3DXCOLOR BulletDebugDrawer::btVector3ToColor(const btVector3& btVec3)
{
	D3DXCOLOR color;
	color.r = btVec3.x();
	color.g = btVec3.y();
	color.b = btVec3.z();
	color.a = 1.0f;
	return color;
}


// ���̕`��
void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    if (rand() % 10 == 0)
    {
        new DebugDrawLine(
            btVector3ToIexVector3(from),
            btVector3ToIexVector3(to),
            0.08f,
            COLORf(0.5f, color.x(), color.y(), color.z()),
            1
            );
    }

    //D3DXVECTOR3 *p;
    //m_Vb->Lock(0, 0, (void **)&p, 0);
    //p[0] = btVector3ToVector3(from);
    //p[1] = btVector3ToVector3(to);
    //m_Vb->Unlock();

    //D3DMATERIAL9 m = { 0 };
    //m.Diffuse = m.Ambient = D3DXCOLOR(color);
    //m_pDevice->SetMaterial(&m);

    //m_pDevice->SetFVF(D3DFVF_XYZ);
    //m_pDevice->SetStreamSource(0, m_Vb, 0, sizeof(D3DXVECTOR3));
    //m_pDevice->SetTransform(D3DTS_WORLD, &D3DXMATRIX(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
    //m_pDevice->DrawPrimitive(D3DPT_LINELIST, 0, 1);

	//// ���̕`��
	//_DDM::I().DrawLine(
	//	m_pDevice,
	//	btVector3ToVector3(from),
	//	btVector3ToVector3(to),
	//	btVector3ToColor(color)
	//	);
}


// �Փ˓_(�����t��)�̕`��
void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	btVector3 to = distance * (PointOnB + normalOnB);
	drawLine(PointOnB, to, color);
}


// �x���A�G���[���̏o��
void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
	MyDebugString(warningString);
}


// 3D�����̕`��
void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}


// �f�o�b�O���[�h�̐ݒ�
void BulletDebugDrawer::setDebugMode(int debugDrawMode)
{
	m_debugDrawMode = debugDrawMode;
}


// �f�o�b�O���[�h�̎擾
int BulletDebugDrawer::getDebugMode() const
{
	return m_debugDrawMode;
}
