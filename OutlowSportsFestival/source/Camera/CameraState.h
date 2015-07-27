#ifndef __CAMERA_STATE_H__
#define __CAMERA_STATE_H__

#include "Camera.h"

//*********************************************************
//	カメラの動きクラスヘッダ
//*********************************************************

//試合時のカメラステート
class CameraStateGamePlay :public CameraState
{
public:
	void Enter(Camera* c);
	void Execute(Camera* c);
	void Exit(Camera* c);

private:

};


//目標に向かって移動
class CameraStateMovetoPoint :public CameraState
{
public:
	enum class CalcType
	{
		_Linear, //線形
		_Cos     //cos式
	};

	CameraStateMovetoPoint(
		CrVector3      pos,
		CrVector3      target,
		float          speed,
		UINT           frame,
		CalcType       calcType,
		CameraState*   pNextState
		);

	~CameraStateMovetoPoint();

private:
	Vector3        m_Pos;
	Vector3        m_Target;
	float          m_Speed;
	CameraState*   m_pNextState;
	UINT           m_Frame;
	CalcType       m_CalcType;

	UINT           m_FrameCount;

	void Enter(Camera* c);
	void Execute(Camera* c);
	void Exit(Camera* c);
};

#endif