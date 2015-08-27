#ifndef __CAMERA_STATE_H__
#define __CAMERA_STATE_H__

#include "Camera.h"
#include "../GameSystem/ForwardDecl.h"


//*********************************************************
//	カメラの動きクラスヘッダ
//*********************************************************

//試合時のカメラステート
class CameraStateGamePlay :public CameraState
{
public:
    CameraStateGamePlay(bool pos_reset = false);

	void Enter(Camera* c);
	void Execute(Camera* c);
	void Exit(Camera* c);

private:

    float m_PosSpeed;
    float m_TargetSpeed;
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


//デバッグ用自由移動カメラ
class CameraStateFreeMove :public CameraState
{
public:
    void Enter(Camera* c);
    void Execute(Camera* c);
    void Exit(Camera* c);
private:
    Vector2 GetKeyWASD()const;
    Vector2 GetRightStick()const;
};


//キャラクタにズームするカメラ
class CameraStateCharacterZoom :public CameraState
{
public:
    CameraStateCharacterZoom(
        LpCharacterBase  pZoomCharacter,
        RATIO            speed
        );

    void Enter(Camera* c);
    void Execute(Camera* c);
    void Exit(Camera* c);

private:
    LpCharacterBase  m_pZoomCharacter;
    RATIO            m_Speed;
};


#endif