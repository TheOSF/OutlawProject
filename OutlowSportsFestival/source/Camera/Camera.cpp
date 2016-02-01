#include "Camera.h"
#include "CameraState.h"
#include "../IexSystem/System.h"
#include "../../IEX/IEX_Input.h"

Camera* Camera::m_pInstance = nullptr;

static float fRand(float fmin, float fmax)
{
	return ((float)rand() / RAND_MAX)*(fmax - fmin) + fmin;
}

CameraDrawObject::CameraDrawObject()
{
    m_isDraw = true;
    m_Pos = Vector3Zero;
    m_DrawImportant = 1.0f;

    DefCamera.m_DrawObjectList.push_back(this);
}

CameraDrawObject::~CameraDrawObject()
{
    Camera& C = DefCamera;

    for (auto it = C.m_DrawObjectList.begin();
        it != C.m_DrawObjectList.end();
        ++it
        )
    {
        if (*it == this)
        {
            C.m_DrawObjectList.erase(it);
            return;
        }
    }

    MyAssert(false, "�폜���s");
}


//------------------------------//

Camera& Camera::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Camera();
	}
	return *m_pInstance;
}

void Camera::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

//�}�C�t���[���̍X�V
void Camera::Update()
{
	Vector3 shock_vec, temp_pos, temp_target;

	//�X�e�[�g�����s
	m_pStateMachine->state_execute();

	//�Ռ��ɂ��Y�����擾
	ShockUpdate(shock_vec);

	//���݂̍��W�ɏՌ��𑫂��čs����X�V
	temp_pos = m_Position + shock_vec;
	temp_target = m_Target + shock_vec;

	UpdateMatrix(temp_pos, temp_target);

    shader->SetValue("g_CameraPos", temp_pos);

    //if (KEY(KEY_ENTER, 0) == 3)
    //{
    //    WriteParam();
    //}
}

//��ʏ���
void Camera::Clear(DWORD color, bool clear_z)
{
	m_IexView.Clear(color, clear_z);
}

//�V���������N���X���Z�b�g
void Camera::SetNewState(CameraState* pNewState)
{
	m_pStateMachine->set_state(pNewState);
}

//�J�����̐U����^����
void Camera::SetShock(Vector2 power, float frame)
{
    //���݂̒l�̂ق����傫�������ꍇ�͏������Ȃ�
    if ((ShockParam.time / ShockParam.max_time) * Vector2Length(ShockParam.power) > Vector2Length(power))
    {
        return;
    }

	ShockParam.power = power;
	ShockParam.time  = frame;
	ShockParam.max_time = frame;
}


//���[���h���W���v���W�F�N�V������ԍ��W�ɕϊ�����
bool Camera::WorldToProjection(Vector3* pOut, CrVector3 v)
{
    const Matrix& m = m_VP;
    Vector3 p(
        v.x*m._11 + v.y*m._21 + v.z*m._31 + m._41,
        v.x*m._12 + v.y*m._22 + v.z*m._32 + m._42,
        v.x*m._13 + v.y*m._23 + v.z*m._33 + m._43
        );
    const float w = v.x*m._14 + v.y*m._24 + v.z*m._34 + m._44;
    
    p.x /= w;
    p.y /= w;

    *pOut = p;

    return
        p.z > 0 &&
        p.x < 1 && p.x > -1 &&
        p.y < 1 && p.y > -1;
}

//�v���W�F�N�V������ԍ��W�����[���h���W�ɕϊ�����
void Camera::ProjectionToWorld(Vector3* pOut, CrVector3 In)
{
	*pOut = Vector3MulMatrix(In, m_VP_inv);
}

//�r���[��ԏ�ł�Z���W�𓾂�
float Camera::GetCameraZ(CrVector3 in)const
{
    return in.x*matView._13 + in.y*matView._23 + in.z*matView._33 + matView._43;
}

const Matrix& Camera::GetBillbordMatrix()const
{
	return m_Billbord;
}

//�r���[�v���W�F�N�V�����ϊ�������s��𓾂�
const Matrix& Camera::GetViewProjectionMatrix()const
{
    return m_VP;
}

//�t�r���[�v���W�F�N�V�����ϊ�������s��𓾂�
const Matrix& Camera::GetInvViewProjectionMatrix()const
{
    return m_VP_inv;
}

CrVector3 Camera::GetRight()const
{
	return m_Right;
}

CrVector3 Camera::GetUp()const
{
	return m_Up;
}

CrVector3 Camera::GetForward()const
{
	return m_Forward;
}

const Camera::CameraDrawObjectList& Camera::GetCameraDrawObjectList()
{
    return m_DrawObjectList;
}

Camera::Camera()
{
	m_pStateMachine = new CameraStateMachine(this);
	//m_pStateMachine->set_state(new CameraStateGamePlay());

	ShockParam.power = Vector2(0, 0);
	ShockParam.time = 0;
	ShockParam.max_time = 0;

    m_IexView.SetProjection(D3DX_PI /4, 2.5f, 500);
}

Camera::~Camera()
{
	delete m_pStateMachine;
}

//�J�����s����X�V����
void Camera::UpdateMatrix(Vector3 pos, Vector3 target)
{
	m_IexView.Set(pos, target);
	m_IexView.Activate();

	m_Billbord = matView;
	m_Billbord._14 = m_Billbord._24 = m_Billbord._34 = 0.0f;

	D3DXMatrixInverse(&m_Billbord, 0, &m_Billbord);

	m_VP = matView;
	m_VP *= matProjection;

	D3DXMatrixInverse(&m_VP_inv, 0, &m_VP);

	m_Right.x = matView._11;
	m_Right.y = matView._21;
	m_Right.z = matView._31;

	m_Up.x = matView._12;
	m_Up.y = matView._22;
	m_Up.z = matView._32;

	m_Forward.x = matView._13;
	m_Forward.y = matView._23;
	m_Forward.z = matView._33;


	m_Right.Normalize();
	m_Up.Normalize();
	m_Forward.Normalize();
}

void Camera::WriteParam()
{
    FILE* fp = nullptr;

    if (fopen_s(&fp, "DATA\\CameraParams.txt", "a+") != 0)
    {
        return;
    }

    fprintf(
        fp,
        "P(%f,%f,%f) T(%f,%f,%f)\n",
        m_Position.x, m_Position.y, m_Position.z,
        m_Target.x, m_Target.y, m_Target.z
        );

    fclose(fp);

}

//�Ռ����X�V����
void Camera::ShockUpdate(Vector3& ShockVector)
{
	//������
	ShockVector = Vector3Zero;

	//�U�����Ă��Ȃ��ꍇ��return
	if (ShockParam.max_time <= 0 || 
		ShockParam.time <= 0)
	{
		return;
	}

	//���Ԃ��i�ނɂ�Ďキ
	const float t = ShockParam.time / ShockParam.max_time;

	//�ړ�
	ShockVector += m_Right*ShockParam.power.x*t*fRand(-1, 1);
	ShockVector += m_Up*ShockParam.power.y*t*fRand(-1, 1);

	//���Ԍo��
	--ShockParam.time;
}