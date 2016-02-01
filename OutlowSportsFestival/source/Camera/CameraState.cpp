#include "CameraState.h"
#include "../character/CharacterManager.h"
#include "../character/CharacterFunction.h"
#include "../character/CharacterBase.h"
#include "../GameSystem/GameController.h"
#include "IEX_Input.h"

//試合時のカメラステート

const Vector3 CameraStateGamePlay::first_pos(0, 40, -55);

CameraStateGamePlay::CameraStateGamePlay(bool pos_reset) :
m_pCamera(nullptr)
{
    //m_PosSpeed = 0.04f;
    //m_TargetSpeed = 0.02f;

    m_PosSpeed = 0.01f;
    m_TargetSpeed = 0.01f;

    if (pos_reset)
    {
        m_PosSpeed = 1;
        m_TargetSpeed = 1;
    }
}

void CameraStateGamePlay::Enter(Camera* c)
{
    m_pCamera = c;
}

void CameraStateGamePlay::Execute(Camera* c)
{
    const Camera::CameraDrawObjectList& DrawList = c->GetCameraDrawObjectList();

	//キャラクタがいない場合移動できない
    if (DrawList.empty())
	{
		return;
	}

	Vector3 center(0, 0, 0);
	int livecnt = 0;
	const float viewangle = PI / 4.5f;	//カメラの視野角
	float dRightLen = 0;

	//全キャラクタ座標の平均座標を算出する
    for (auto& it : DrawList)
	{
        if (it->m_isDraw == false) continue;
        center += it->m_Pos;
		livecnt++;
	}

	if (livecnt > 1)
	{
		center /= (float)livecnt;
	}

	center.z -= 6.0f;	//若干手前に

	//カメラのターゲットを補間
    c->m_Target += (center - c->m_Target)*m_TargetSpeed;

    Vector3 mVec = first_pos - center;
    Vector3 moveTarget;
    Vector3 LineMostNear;

    mVec.Normalize();

    
	//どれだけカメラを引くかを算出する
    for (auto& it : DrawList)
	{
        if (it->m_isDraw == false)continue;
		CrVector3 chrpos = it->m_Pos;

		//カメラ注視点からの距離を求める
		float rlen = Vector3Length(chrpos - center);
		rlen += 12;	//ちょっと大きめに引くため

		if (dRightLen < rlen)dRightLen = rlen;	//最大なら更新
	}

	//視野角のtanから目標を視野に入れるための後方距離を算出
	float b = dRightLen / tanf(viewangle);

	//移動目標を計算
	moveTarget = center + mVec*b;

    float L = Vector3Distance(center, moveTarget);
    if (L < 30.0f)
    {
        moveTarget = center + Vector3Normalize(moveTarget-center)*30.0f;
    }

	//補間する
    c->m_Position += (moveTarget - c->m_Position)*m_PosSpeed;

#ifdef _DEBUG
    if (GetKeyState('Q') & 0x80)
    {
        c->SetNewState(new CameraStateFreeMove());
    }
#endif

    m_PosSpeed = 0.05f;
    m_TargetSpeed = 0.04f;

}


//void CameraStateGamePlay::Execute(Camera* c)
//{
//
//	const CharacterManager::CharacterMap& chr_map = DefCharacterMgr.GetCharacterMap();
//
//	//キャラクタがいない場合移動できない
//	if (chr_map.empty())
//	{
//		return;
//	}
//
//	Vector3 center(0, 0, 0);
//	int livecnt = 0;
//    const float viewangle = D3DXToRadian(45);
//	float dRightLen = 0;
//
//	//全キャラクタ座標の平均座標を算出する
//	for (auto it = chr_map.begin();
//		it != chr_map.end();
//		++it)
//	{
//		if (chr_func::isDie(it->first))continue;
//		center += it->first->m_Params.pos;
//		livecnt++;
//	}
//
//	if (livecnt > 1)
//	{
//		center /= (float)livecnt;
//	}
//
//    {
//        Vector3 axis;
//        Vector3Cross(axis, Vector3AxisY, center - first_pos);
//
//        Vector3 NewCenter = Vector3RotateAxis(axis, D3DXToRadian(1), center - first_pos) + first_pos;
//        center = NewCenter;
//        //center = center - first_pos + first_pos;
//    }
//
//	//カメラのターゲットを補間
//    c->m_Target += (center - c->m_Target)*m_TargetSpeed;
//
//    MoveCameraPosition(center, m_pCamera);
//
//#ifdef _DEBUG
//    if (GetKeyState('Q') & 0x80)
//    {
//        c->SetNewState(new CameraStateFreeMove());
//    }
//#endif
//
//    //m_PosSpeed = 0.05f;
//    //m_TargetSpeed = 0.04f;
//
//    m_PosSpeed = 0.01f;
//    m_TargetSpeed = 0.01f;
//
//    //m_PosSpeed = 1;
//    //m_TargetSpeed = 1;
//
//}

void CameraStateGamePlay::Exit(Camera* c)
{


}

//void CameraStateGamePlay::MoveCameraPosition(CrVector3 ViewTarget, Camera* c)
//{
//    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
//    Vector3 MoveTarget, Work;
//    float   BackValue = 0;
//    bool    AllIn = true;
//    
//    for (auto &it : ChrMap)
//    {
//        if (it.first->m_Params.camera_draw == false)
//        {
//            continue;
//        }
//
//        if (c->WorldToProjection(&Work, it.first->m_Params.pos) == false ||
//            Vector3XYLength(Work) > 0.75f || 
//            Work.y < -0.8f)
//        {
//            AllIn = false;
//            BackValue = max(BackValue, max(fabsf(Work.x), fabsf(Work.y)));
//        }
//    }
//
//    if (AllIn)
//    {
//        //補間する
//        c->m_Position += (ViewTarget - c->m_Position)*m_PosSpeed;
//    }
//    else
//    {
//        //補間する
//        c->m_Position += (first_pos - ViewTarget)*m_PosSpeed;
//    }
//
//}

bool CameraStateGamePlay::isViewIn(CrVector3 pos)
{
    if (m_pCamera->WorldToProjection(&m_Work, pos) == false)
    {
        return false;
    }

    return m_Work.y > -0.8f;
}



//目標に向かって２次式で移動ステート

CameraStateMovetoPoint::CameraStateMovetoPoint(
	CrVector3      pos,
	CrVector3      target,
	float          speed,
	UINT           frame,
	CalcType       calcType,
	CameraState*   pNextState
	) :
	m_Pos(pos),
	m_Target(target),
	m_Speed(speed),
	m_Frame(frame),
	m_pNextState(pNextState),
	m_FrameCount(0),
	m_CalcType(calcType)
{

}

CameraStateMovetoPoint::~CameraStateMovetoPoint()
{
	delete m_pNextState;
}

void CameraStateMovetoPoint::Enter(Camera* c)
{

}

void CameraStateMovetoPoint::Execute(Camera* c)
{
	//線形
	if (m_CalcType == CalcType::_Linear)
	{
		Vector3 vec;

		vec = m_Pos - c->m_Position;
		if (vec.Length() > m_Speed)
		{
			vec.Normalize();
			vec *= m_Speed;
		}
		c->m_Position += vec;

		vec = m_Target - c->m_Target;
		if (vec.Length() > m_Speed)
		{
			vec.Normalize();
			vec *= m_Speed;
		}
		c->m_Target += vec;
	}

	//cos式
	if (m_CalcType == CalcType::_Cos)
	{
		c->m_Position += (m_Pos - c->m_Position)*m_Speed;
		c->m_Target += (m_Target - c->m_Target)*m_Speed;
	}


	//ステート移行
	if (++m_FrameCount > m_Frame)
	{
		CameraState* tmp = m_pNextState;

		m_pNextState = nullptr;
		c->SetNewState(tmp);
	}
}


void CameraStateMovetoPoint::Exit(Camera* c)
{

}


//デバッグ用自由移動カメラ
void CameraStateFreeMove::Enter(Camera* c)
{

}

void CameraStateFreeMove::Execute(Camera* c)
{
    Vector3 front = Vector3Normalize(c->m_Target - c->m_Position);
    Vector3 right;

    Vector2 keyL = controller::GetStickValue(controller::stick::left,0);
    Vector2 keyR = controller::GetStickValue(controller::stick::right,0);

    Vector3Cross(right, Vector3AxisY, front);
    right.Normalize();

    c->m_Target += front*keyL.y + right*keyL.x;
    c->m_Position += front*keyL.y + right*keyL.x;

    if (fabsf(keyR.x) > 0.2f)
    {
        c->m_Target = c->m_Position + Vector3RotateAxis(Vector3AxisY, keyR.x*0.01f, front);
    }
    else if (fabsf(keyR.y) > 0.2f)
    {
        c->m_Target = c->m_Position + Vector3RotateAxis(right, keyR.y*0.01f, front);
    }
    
    


#ifdef _DEBUG
    if (GetKeyState('E') & 0x80)
    {
        c->SetNewState(new CameraStateGamePlay());
    }
#endif
}

void CameraStateFreeMove::Exit(Camera* c)
{

}


Vector2 CameraStateFreeMove::GetKeyWASD()const
{
    Vector2 ret;
    
    ret.x = (float)((GetKeyState('D') & 0x80) != 0) - (float)((GetKeyState('A') & 0x80) != 0);
    ret.y = (float)((GetKeyState('W') & 0x80) != 0) - (float)((GetKeyState('S') & 0x80) != 0);

    return ret;
}




//-----------------------------------------------------------------//


//キャラクタにズームするカメラ
CameraStateCharacterZoom::CameraStateCharacterZoom(
    LpCharacterBase  pZoomCharacter,
    RATIO            speed
    ) :
    m_pZoomCharacter(pZoomCharacter),
    m_Speed(speed)
{

}

void CameraStateCharacterZoom::Enter(Camera* c)
{

}

void CameraStateCharacterZoom::Execute(Camera* c)
{
    const Vector3 target = m_pZoomCharacter->m_Params.pos + Vector3(0, 3, 0);
    const Vector3 pos = target + Vector3(0, 2, -13);


    c->m_Position += (pos - c->m_Position)*m_Speed;
    c->m_Target  += (target - c->m_Target)*m_Speed;

}

void CameraStateCharacterZoom::Exit(Camera* c)
{

}

//スキル時にキャラクタにズームするカメラ
CameraStateSkillCharacterZoom::CameraStateSkillCharacterZoom(
    LpCharacterBase  pZoomCharacter,
    RATIO            speed,
    int              time
    ) :
    m_pZoomCharacter(pZoomCharacter),
    m_Speed(speed),
    m_Timer(time),
    m_MoveAngle(0)
{

}

void CameraStateSkillCharacterZoom::Enter(Camera* c)
{
    const float MoveLen = 15.0f;  //スーム距離
    const float DontZoomLen = 40.0f; //ズームしないようになる距離
    Vector3 TargetPos = GetTargetPos();
    Vector3 ToTarget = GetTargetPos() - c->m_Position;
    float Len;

    m_FirstViewVec = c->m_Target - c->m_Position;

    {
        m_MovePos = c->m_Position + Vector3Normalize(ToTarget)*MoveLen;
        
        Len = Vector3Distance(m_MovePos, TargetPos);

        if (Len < DontZoomLen)
        {
            m_MovePos -= Vector3Normalize(ToTarget) * (DontZoomLen - Len);
            
            if (Vector3Distance(m_MovePos, TargetPos) > ToTarget.Length())
            {
                m_MovePos = c->m_Position;
            }
            //m_MovePos = TargetPos - Vector3Normalize(m_FirstViewVec)*DontZoomLen;
        }
    }
}

void CameraStateSkillCharacterZoom::Execute(Camera* c)
{
    const Vector3 target = GetTargetPos();
    RADIAN MoveMaxAngle = D3DXToRadian(8);

    if (--m_Timer < 0)
    {
        c->SetNewState(new CameraStateGamePlay());
    }

    {
        Vector3 v = target - c->m_Position;
        Vector3Cross(m_RotateAxis, m_FirstViewVec, v);

        m_RotateAxis.Normalize();

        RADIAN Angle = Vector3Radian(m_FirstViewVec, v);

        if (MoveMaxAngle>Angle)
        {
            MoveMaxAngle = Angle;
        }
    }

    m_MoveAngle += (MoveMaxAngle - m_MoveAngle)*0.1f;

    c->m_Position += (m_MovePos - c->m_Position)*0.1f;
    {
        c->m_Target = c->m_Position + Vector3RotateAxis(m_RotateAxis, m_MoveAngle, m_FirstViewVec);
    }
}

void CameraStateSkillCharacterZoom::Exit(Camera* c)
{

}


Vector3 CameraStateSkillCharacterZoom::GetTargetPos()const
{
    return m_pZoomCharacter->m_Params.pos + Vector3(0, 1.5f, 0);
}


//---------------------------------------------------------------------------//


//スキル時にキャラクタにズームするカメラ
CameraStateSkillCharacterZoom2::CameraStateSkillCharacterZoom2(
    LpCharacterBase  pZoomCharacter
    ) :
    m_pZoomCharacter(pZoomCharacter),
    m_Timer(0)
{

}

void CameraStateSkillCharacterZoom2::Enter(Camera* c)
{
    
}

void CameraStateSkillCharacterZoom2::Execute(Camera* c)
{
    
}

void CameraStateSkillCharacterZoom2::Exit(Camera* c)
{

}


Vector3 CameraStateSkillCharacterZoom2::GetTargetPos()const
{
    return m_pZoomCharacter->m_Params.pos + Vector3(0, 1.5f, 0);
}


void CameraStateSkillCharacterZoom2::SetState(StateFunc pSetState)
{

}

void CameraStateSkillCharacterZoom2::State_AngleMove()
{

}

void CameraStateSkillCharacterZoom2::State_ZoomChr()
{

}