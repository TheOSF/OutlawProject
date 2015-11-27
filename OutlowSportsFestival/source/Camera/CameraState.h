#ifndef __CAMERA_STATE_H__
#define __CAMERA_STATE_H__

#include "Camera.h"
#include "../GameSystem/ForwardDecl.h"


//*********************************************************
//	�J�����̓����N���X�w�b�_
//*********************************************************

//�������̃J�����X�e�[�g
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


//�ڕW�Ɍ������Ĉړ�
class CameraStateMovetoPoint :public CameraState
{
public:
	enum class CalcType
	{
		_Linear, //���`
		_Cos     //cos��
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


//�f�o�b�O�p���R�ړ��J����
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


//�L�����N�^�ɃY�[������J����
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

//�X�L�����ɃL�����N�^�ɃY�[������J����
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