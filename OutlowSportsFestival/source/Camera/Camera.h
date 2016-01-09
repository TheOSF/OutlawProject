#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "iextreme.h"
#include "../utillity/StateTemplate.h"

class Camera;
typedef StateMachine<Camera, void> CameraStateMachine;
typedef State<Camera, void> CameraState;

//***********************************************
//	カメラクラス(シングルトン)
//***********************************************

class Camera
{
public:
	static Camera& GetInstance();
	static void Release();

	// 毎フレームの更新
	void Update();

	//画面消去
	void Clear(DWORD color = 0, bool clear_z = true);

	//新しい動きクラスをセット
	void SetNewState(CameraState* pNewState);

	//カメラの振動を与える
	void SetShock(Vector2 power, float frame);

	//ワールド座標をプロジェクション空間座標に変換する(wで除算済み)
	bool WorldToProjection(Vector3* pOut, CrVector3 In);

	//プロジェクション空間座標をワールド座標に変換する
	void ProjectionToWorld(Vector3* pOut, CrVector3 In);

    //ビュー空間上でのZ座標を得る
    float GetCameraZ(CrVector3 in)const;

	//ビルボード計算用行列を得る
	const Matrix& GetBillbordMatrix()const;

    //ビュープロジェクション変換をする行列を得る
    const Matrix& GetViewProjectionMatrix()const;
        
    //逆ビュープロジェクション変換をする行列を得る
    const Matrix& GetInvViewProjectionMatrix()const;

	//カメラの各方向ベクトルを得る
	CrVector3 GetRight()const;
	CrVector3 GetUp()const;
	CrVector3 GetForward()const;

	//位置と見る向き
	Vector3 m_Target;
	Vector3 m_Position;

private:
	Camera();
	Camera(const Camera&);
	~Camera();


    void WriteParam();

	//カメラ行列を更新する
	void UpdateMatrix(Vector3 pos, Vector3 target);

	//衝撃を更新する
	void ShockUpdate(Vector3& ShockVector);

	static Camera*			m_pInstance;

	CameraStateMachine*		m_pStateMachine;
	iexView					m_IexView;
	Matrix					m_VP;
	Matrix					m_VP_inv;
	Matrix                  m_Billbord;
	Vector3					m_Right, m_Up, m_Forward;
	
	struct
	{
		Vector2 power;
		float	time;
		float	max_time;
	}
	ShockParam;
};

#define DefCamera (Camera::GetInstance())

#endif