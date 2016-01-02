#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameController.h"
#include <array>

//--------------------------------------------------------//
// キャラクタセレクト画面でのカーソル関連のクラスヘッダ
//--------------------------------------------------------//

class CursorManager;
class CursorObject;

//カーソル移動ポイント
class CursorPoint
{
public:

    //選択したときのイベントクラス
    class SelectEvent
    {
    public:
        //イベントタイプ
        enum class EventType
        {
            Select,
            Cancel,
            
        };
        virtual ~SelectEvent(){}

        //なんか押した時に呼ばれる
        virtual bool Select(CursorObject* pCur, EventType type) = 0;
    };

    enum : size_t
    {
        MaxCursor = 4
    };


    CursorPoint(CursorManager* pManager);
    ~CursorPoint();

    Vector2   m_Pos;   //位置
    bool      m_Only;  //カーソルがひとつしかさせないかどうか

    Vector2 CalcCursorPos(CursorObject* p);  //カーソルの位置を得る

    bool OnCursor(CursorObject* p);    //カーソルが上に乗った(戻り値：乗れたかどうか)
    bool LeaveCursor(CursorObject* p); //離れた

private:
    
    typedef std::array<CursorObject*, MaxCursor> CursorArray;

    CursorArray          m_CursorArray;
    CursorManager* const m_pManager;
    int                  m_OnCursorNum;

    Vector2 CalcCurosrPosByCursorID(int Id);
};



//カーソルクラス
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



//カーソルマネージャ
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