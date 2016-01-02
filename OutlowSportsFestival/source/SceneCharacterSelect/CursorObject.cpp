#include "CursorObject.h"
#include "../Sound/Sound.h"

CursorPoint::CursorPoint(CursorManager* pManager) :
m_pManager(pManager),
m_Pos(100, 100),
m_Only(false),
m_OnCursorNum(0)
{
    m_CursorArray.fill(nullptr);

    m_pManager->Regist(this);
}

CursorPoint::~CursorPoint()
{
    for (auto& it : m_CursorArray)
    {
        if (it != nullptr)
        {
            it->m_pPoint = nullptr;
        }
    }

    m_pManager->Erace(this);
}

//�J�[�\���̈ʒu�𓾂�
Vector2 CursorPoint::CalcCursorPos(CursorObject* p)
{
    int x = 0;

    for (auto& it : m_CursorArray)
    {
        if (it == p)
        {
            break;
        }

        ++x;
    }

    return CalcCurosrPosByCursorID(x);
}

//�J�[�\������ɏ����(�߂�l�F��ꂽ���ǂ���)
bool CursorPoint::OnCursor(CursorObject* p)
{
    //�ЂƂ������Ȃ��ꍇ
    if (m_Only)
    {
        for (auto& it : m_CursorArray)
        {
            //���łɏ���Ă�����false��Ԃ�
            if (it != nullptr)
            {
                return false;
            }
        }

        ++m_OnCursorNum;
        m_CursorArray.at(0) = p;
        return true;
    }


    //�󂫂�T��
    for (auto& it : m_CursorArray)
    {
        if (it == nullptr)
        {
            it = p;
            ++m_OnCursorNum;
            return true;
        }
    }

    //�󂫂���������(MaxCursor�ȏ����Ă���
    return false;
}

bool CursorPoint::LeaveCursor(CursorObject* p) //���ꂽ
{
    //�T��
    for (auto& it : m_CursorArray)
    {
        if (it == p)
        {
            it = nullptr;

            MyAssert(m_OnCursorNum > 0, "����Ă�J�[�\��������������");

            --m_OnCursorNum;


            return true;
        }
    }

    MyAssert(false, "�o�^����Ă��Ȃ��J�[�\���������ɓn����܂���");

    //��������(�o�O�����
    return false;
}

Vector2 CursorPoint::CalcCurosrPosByCursorID(int Id)
{
    const float Size = 50.0f;

    switch (m_OnCursorNum)
    {
    case 1:
        return m_Pos;

    case 2:
        return m_Pos + Vector2((float)(Id * 2 - 1)*Size, 0);

    case 3:

        if (Id < 2)
        {
            return m_Pos + Vector2((float)(Id * 2 - 1)*Size, Size*0.5f);
        }
        else
        {
            return m_Pos - Vector2(0, Size*0.5f);
        }

    case 4:
        return m_Pos + Vector2((float)(Id % 2 * 2 - 1), (float)(Id / 2 * 2 - 1))*Size;

    }
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

//�J�[�\���N���X
CursorObject::CursorObject(
    CursorManager*              pManager,
    CursorPoint*                pInitPoint,
    controller::CONTROLLER_NUM  ControllerNum) :

m_pManager(pManager),
m_pPoint(pInitPoint),
m_Lock(false),
m_ControllerNum(ControllerNum)
{
    //�����ʒu�ݒ�
    m_Pos = (m_pPoint != nullptr) ? (m_pPoint->m_Pos) : (Vector2(100, 100));

    //�}�l�[�W���ɓo�^
    m_pManager->Regist(this);
}

CursorObject::~CursorObject()
{
    //�}�l�[�W������폜
    m_pManager->Erace(this);
}

bool CursorObject::Move(CrVector2 Vec)
{
    CursorPoint* pMovePoint = nullptr;
    bool ret = m_pManager->Move(m_pPoint, Vec, &pMovePoint);

    if (ret)
    {
        m_pPoint = pMovePoint;
    }

    return ret;
}

bool CursorObject::Update()
{
    //�J�[�\�������݂��Ȃ�����
    if (m_pPoint == nullptr && m_pManager->GetNearPoint(m_Pos, &m_pPoint) == false)
    {
        return true;
    }

    //�X�e�B�b�N���͂��������ꍇ
    if (controller::GetStickJustMove(controller::stick::left, m_ControllerNum))
    {
        Vector2 StickValue = controller::GetStickValue(controller::stick::left, m_ControllerNum);
        StickValue.y = -StickValue.y;

        //�ړ��ɐ��������ꍇ
        if (Move(StickValue))
        {
            //�����Ȃ炷��
            Sound::Play(Sound::Cursor_move);
        }
    }

    //�ړ���̃|�C���g�Ɍ����Ĉړ�����
    m_Pos += (m_pPoint->CalcCursorPos(this) - m_Pos)*0.2f;
    
    return true;
}

bool CursorObject::Msg(MsgType mt)
{

    return false;
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------


//�J�[�\���}�l�[�W��
CursorManager::CursorManager()
{
    m_CursorArray.fill(nullptr);
    m_CursorPointArray.fill(nullptr);
}

CursorManager::~CursorManager()
{
    for (auto& it : m_CursorPointArray)
    {
        delete it;
    }
}

bool CursorManager::GetNearPoint(CrVector2 NowPos, CursorPoint** ppOut)
{
    float MostNear = 100000.0f, TempLen;
    *ppOut = nullptr;

    for (auto& it : m_CursorPointArray)
    {
        if (it == nullptr)
        {
            continue;
        }

        TempLen = Vector2Length(NowPos - it->m_Pos);

        if (TempLen < MostNear)
        {
            MostNear = TempLen;
            *ppOut = it;
        }
    }

    return *ppOut != nullptr;
}

bool CursorManager::Move(CursorPoint* pNowPoint, CrVector2 Vec, CursorPoint** ppOut)
{
    const RADIAN OkAngle = D3DXToRadian(45);  //���m����p�x
    RADIAN angle;
    float  MostNear = 1000000.0f, TempLen;
    Vector2 v;

    *ppOut = nullptr;

    for (auto& it : m_CursorPointArray)
    {
        if (it == nullptr || it == pNowPoint)
        {
            continue;
        }

        v = it->m_Pos - pNowPoint->m_Pos;

        angle = Vector2Radian(v, Vec);

        TempLen = Vector2Length(v);

        if (angle > OkAngle || TempLen > MostNear)
        {
            continue;
        }

        MostNear = TempLen;
        *ppOut = it;
    }

    return *ppOut != nullptr;
}

void CursorManager::Regist(CursorPoint* pPoint)
{
    //�󂫂�T��
    for (auto& it : m_CursorPointArray)
    {
        if (it == nullptr)
        {
            it = pPoint;
            return;
        }
    }

    //�󂫂���������
    MyAssert(false, "�󂫂��Ȃ�����");
}

void CursorManager::Erace(CursorPoint*  pPoint)
{
    //�T��
    for (auto& it : m_CursorPointArray)
    {
        if (it == pPoint)
        {
            it = nullptr;
            return;
        }
    }

    MyAssert(false, "�o�^����Ă��Ȃ��|�C���g�������ɓn����܂���");
}


void CursorManager::Regist(CursorObject* pCursor)
{
    //�󂫂�T��
    for (auto& it : m_CursorArray)
    {
        if (it == nullptr)
        {
            it = pCursor;
            return;
        }
    }

    //�󂫂���������
    MyAssert(false, "�󂫂��Ȃ�����");
}

void CursorManager::Erace(CursorObject*  pCursor)
{
    //�T��
    for (auto& it : m_CursorArray)
    {
        if (it == pCursor)
        {
            it = nullptr;
            return;
        }
    }

    MyAssert(false, "�o�^����Ă��Ȃ��J�[�\���������ɓn����܂���");
}