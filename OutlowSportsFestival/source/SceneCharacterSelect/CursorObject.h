#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameController.h"
#include "../GameScene/SceneGamePlay.h"
#include "../Render/Renderer.h"
#include <array>
#include <list>

class SelectCursor;
class CursorManager;

//サークル移動可能ポイント
class SelectPointBase : public UserInterfaceRenderer {
public:
    enum class PointType {
        Tennis,
        Soccer,
        BaseBall,
        AmericanFootBall,

        Random,

        ComputerDefaultPoint,

        ComputerStrong,
        ComputerChrSelect,
    };

    Vector2         m_MoveTargetPos;
    Vector2         m_Pos;
    Vector2         m_Size;

    const PointType m_Type;

    SelectPointBase(
        CursorManager* pMgr,
        PointType      Type,
        LPIEX2DOBJ     pTexture,
        const RectI&   TexRect,
        SelectCursor* pLinkCursor = nullptr,
        SelectPointBase* pLinkCharaPoint = nullptr
        );

    virtual ~SelectPointBase();

    Vector2 GetOffSetPos(SelectCursor* p);

    bool CanOnCursor(SelectCursor* pCursor)const;

    void OnCursor(SelectCursor* p);
    void LeaveCursor(SelectCursor* p);

    void Select(SelectCursor* p);
    void Cancel(SelectCursor* p);

    bool Update();

private:
    std::list<SelectCursor*>  m_OnCursorData;

    CursorManager* const m_pMgr;
    iex2DObj* const      m_pTexture;
    RectI          m_TexRect;
    SelectCursor* m_pLinkCursor;
    SelectPointBase* m_pLinkCharaPoint;

    int GetOnCursorID(SelectCursor* p);

    StrongType::Value GetChangeStrongType(StrongType::Value Now);
    void SetStrongTypePointRect(StrongType::Value Now);

    void CalcZ()override;
    void Render()override;
};


//カーソルクラス
class SelectCursor : public UserInterfaceRenderer {
public:

    sceneGamePlay::InitParams::PlayerInfo   m_PlayerInfo;
    controller::CONTROLLER_NUM              m_ControllerNum;
    Vector2         m_Pos;
    bool            m_Lock;
    bool            m_Selected;
    SelectCursor* m_pControllerCursor;
    bool m_Visible;

    SelectCursor(
        CursorManager*                  pMgr,
        controller::CONTROLLER_NUM      Num,
        LPIEX2DOBJ                      pTexture,
        const RectI&                    TexRect,
        SelectPointBase*                pInitPoint,
        SelectPointBase* pDefaultPoint
        );

    ~SelectCursor();

    void SetPoint(SelectPointBase* pNewPoint);
    bool SelectNowPoint();
    void MoveToDefaultPoint();
    bool Update();

    inline SelectPointBase* GetPoint()
    {
        return m_pPoint;
    }

private:

    CursorManager* const                  m_pMgr;
    iex2DObj* const                       m_pTexture;
    const RectI                           m_TexRect;

    SelectPointBase*                      m_pPoint;
    SelectPointBase* m_pDefaultPoint;

    void Controll();

    void CalcZ()override;
    void Render()override;
};


//マネージャ
class CursorManager : public GameObjectBase {
public:

    CursorManager();
    ~CursorManager();

    bool Update()override;
    bool Msg(MsgType mt)override { return false; }

    void Regist(SelectPointBase* p);
    void Erace(SelectPointBase* p);

    void Regist(SelectCursor* p);
    void Erace(SelectCursor* p);

    bool GetNextPoint(CrVector2 Vec, SelectCursor* pCursor, SelectPointBase* pNow, SelectPointBase** ppOut);

    bool isAllPlayerSelected()const;
    bool isSelected(int player_number)const;

    bool isLocked(int player_number)const;

    void GetData(sceneGamePlay::InitParams& OutData);
    void RandomMove(SelectCursor* p);

private:
    std::list<SelectPointBase*> m_PointData;
    std::list<SelectCursor*>    m_CursorData;
};


