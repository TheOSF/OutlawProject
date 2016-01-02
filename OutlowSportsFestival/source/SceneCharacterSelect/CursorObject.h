#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameController.h"
#include <array>

//--------------------------------------------------------//
// �L�����N�^�Z���N�g��ʂł̃J�[�\���֘A�̃N���X�w�b�_
//--------------------------------------------------------//

class CursorManager;
class CursorObject;

//�J�[�\���ړ��|�C���g
class CursorPoint
{
public:

    //�I�������Ƃ��̃C�x���g�N���X
    class SelectEvent
    {
    public:
        //�C�x���g�^�C�v
        enum class EventType
        {
            Select,
            Cancel,
            
        };
        virtual ~SelectEvent(){}

        //�Ȃ񂩉��������ɌĂ΂��
        virtual bool Select(CursorObject* pCur, EventType type) = 0;
    };

    enum : size_t
    {
        MaxCursor = 4
    };


    CursorPoint(CursorManager* pManager);
    ~CursorPoint();

    Vector2   m_Pos;   //�ʒu
    bool      m_Only;  //�J�[�\�����ЂƂ��������Ȃ����ǂ���

    Vector2 CalcCursorPos(CursorObject* p);  //�J�[�\���̈ʒu�𓾂�

    bool OnCursor(CursorObject* p);    //�J�[�\������ɏ����(�߂�l�F��ꂽ���ǂ���)
    bool LeaveCursor(CursorObject* p); //���ꂽ

private:
    
    typedef std::array<CursorObject*, MaxCursor> CursorArray;

    CursorArray          m_CursorArray;
    CursorManager* const m_pManager;
    int                  m_OnCursorNum;

    Vector2 CalcCurosrPosByCursorID(int Id);
};



//�J�[�\���N���X
class CursorObject :public GameObjectBase
{
public:
    CursorObject(
        CursorManager*              pManager,
        CursorPoint*                pInitPoint,
        controller::CONTROLLER_NUM  ControllerNum
        );

    ~CursorObject();

    bool            m_Lock;
    Vector2         m_Pos;
    CursorPoint*    m_pPoint;

    bool Move(CrVector2 Vec);

private:
    const controller::CONTROLLER_NUM    m_ControllerNum;
    CursorManager* const                m_pManager;

    bool Update()override;
    bool Msg(MsgType mt)override;
};



//�J�[�\���}�l�[�W��
class CursorManager
{
public:
    CursorManager();
    ~CursorManager();

    bool GetNearPoint(CrVector2 NowPos, CursorPoint** ppOut);
    bool Move(CursorPoint* pNowPoint, CrVector2 Vec, CursorPoint** ppOut);

    void Regist(CursorPoint* pPoint);
    void Erace(CursorPoint*  pPoint);

    void Regist(CursorObject* pCursor);
    void Erace(CursorObject*  pCursor);

private:
    std::array<CursorObject*, CursorPoint::MaxCursor>   m_CursorArray;
    std::array<CursorPoint*, 64>                        m_CursorPointArray;
};