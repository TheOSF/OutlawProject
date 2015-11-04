#include "Camera.h"
#include "CameraState.h"
#include "../IexSystem/System.h"

Camera* Camera::m_pInstance = nullptr;

static float fRand(float fmin, float fmax)
{
	return ((float)rand() / RAND_MAX)*(fmax - fmin) + fmin;
}

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

//マイフレームの更新
void Camera::Update()
{
	Vector3 shock_vec, temp_pos, temp_target;

	//ステートを実行
	m_pStateMachine->state_execute();

	//衝撃によるズレを取得
	ShockUpdate(shock_vec);

	//現在の座標に衝撃を足して行列を更新
	temp_pos = m_Position + shock_vec;
	temp_target = m_Target + shock_vec;

	UpdateMatrix(temp_pos, temp_target);

    shader->SetValue("g_CameraPos", temp_pos);
}

//画面消去
void Camera::Clear(DWORD color, bool clear_z)
{
	m_IexView.Clear(color, clear_z);
}

//新しい動きクラスをセット
void Camera::SetNewState(CameraState* pNewState)
{
	m_pStateMachine->set_state(pNewState);
}

//カメラの振動を与える
void Camera::SetShock(Vector2 power, float frame)
{
    //現在の値のほうが大きかった場合は処理しない
    if ((ShockParam.time / ShockParam.max_time) * Vector2Length(ShockParam.power) > Vector2Length(power))
    {
        return;
    }

	ShockParam.power = power;
	ShockParam.time  = frame;
	ShockParam.max_time = frame;
}


//ワールド座標をプロジェクション空間座標に変換する
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

//プロジェクション空間座標をワールド座標に変換する
void Camera::ProjectionToWorld(Vector3* pOut, CrVector3 In)
{
	*pOut = Vector3MulMatrix(In, m_VP_inv);
}

//ビュー空間上でのZ座標を得る
float Camera::GetCameraZ(CrVector3 in)const
{
    return in.x*matView._13 + in.y*matView._23 + in.z*matView._33 + matView._43;
}

const Matrix& Camera::GetBillbordMatrix()const
{
	return m_Billbord;
}

//ビュープロジェクション変換をする行列を得る
const Matrix& Camera::GetViewProjectionMatrix()const
{
    return m_VP;
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

Camera::Camera()
{
	m_pStateMachine = new CameraStateMachine(this);
	m_pStateMachine->set_state(new CameraStateGamePlay());

	ShockParam.power = Vector2(0, 0);
	ShockParam.time = 0;
	ShockParam.max_time = 0;

    m_IexView.SetProjection(D3DX_PI /4, 2.5f, 300);
}

Camera::~Camera()
{
	delete m_pStateMachine;
}

//カメラ行列を更新する
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

//衝撃を更新する
void Camera::ShockUpdate(Vector3& ShockVector)
{
	//初期化
	ShockVector = Vector3Zero;

	//振動していない場合はreturn
	if (ShockParam.max_time <= 0 || 
		ShockParam.time <= 0)
	{
		return;
	}

	//時間が進むにつれて弱く
	const float t = ShockParam.time / ShockParam.max_time;

	//移動
	ShockVector += m_Right*ShockParam.power.x*t*fRand(-1, 1);
	ShockVector += m_Up*ShockParam.power.y*t*fRand(-1, 1);

	//時間経過
	--ShockParam.time;
}