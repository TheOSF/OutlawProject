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
    static const Vector3 first_pos;

    CameraStateGamePlay(bool pos_reset = false);

	void Enter(Camera* c);
	void Execute(Camera* c);
	void Exit(Camera* c);

private:
    Camera* m_pCamera;
    Vector3 m_Work;
    float m_PosSpeed;
    float m_TargetSpeed;

    bool isViewIn(CrVector3 pos);
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

//スキル時にキャラクタにズームするカメラ
class CameraStateSkillCharacterZoom :public CameraState
{
public:
    CameraStateSkillCharacterZoom(
        LpCharacterBase  pZoomCharacter,
        RATIO            speed,
        int              time
        );

    void Enter(Camera* c);
    void Execute(Camera* c);
    void Exit(Camera* c);

    Vector3 GetTargetPos()const;

private:
    LpCharacterBase  m_pZoomCharacter;
    Vector3          m_FirstViewVec;
    Vector3          m_RotateAxis;
    RADIAN           m_MoveAngle;
    RATIO            m_Speed;
    int              m_Timer;
    Vector3          m_MovePos;
};

#endif