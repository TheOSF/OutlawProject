#include "CursorObject.h"
#include "../Sound/Sound.h"

//--------------------------------------------------------------------
//                  サークル移動可能ポイント
//--------------------------------------------------------------------

SelectPointBase::SelectPointBase(
    CursorManager* pMgr,
    PointType      Type,
    LPIEX2DOBJ     pTexture,
    const RectI&   TexRect
    ):
    m_Type(Type),
    m_pTexture(pTexture),
    m_TexRect(TexRect),
    m_pMgr(pMgr)
{
    m_MoveTargetPos = Vector2(100, 100);
    m_Pos = Vector2(0, 0);
    m_Size = Vector2(120, 120);

    m_pMgr->Regist(this);
}

SelectPointBase::~SelectPointBase()
{

}

Vector2 SelectPointBase::GetOffSetPos(SelectCursor* p)
{
    const float Size = 30.0f;
    const int Id = GetOnCursorID(p);
    
    switch (m_OnCursorData.size())
    {
    case 1:
        return m_Pos;
    
    case 2:
        return m_Pos + Vector2((float)((Id - 1) * 2 - 1)*Size, 0);
    
    case 3:
    
        if (Id <= 2)
        {
            return m_Pos - Vector2(-(float)((Id - 1) *2 - 1)*Size, Size*0.66f);
        }
        else
        {
            return m_Pos + Vector2(0, Size * 0.66f);
        }
    
    case 4:
        return m_Pos + Vector2((float)((Id - 1) % 2 * 2 - 1), (float)((Id - 1) / 2 * 2 - 1))*Size;
    default:
    
        break;
    }

    MyAssert(false, "乗っている数がおかしい！");

    return Vector2(0, 0);
}

bool SelectPointBase::CanOnCursor()const
{
    //コンピュータ用のポイント出なければ乗れる
    return m_Type != PointType::ComputerDefaultPoint;
}

void SelectPointBase::OnCursor(SelectCursor* p)
{
    m_OnCursorData.push_back(p);

    switch (m_Type)
    {

    case PointType::Tennis:
        p->m_PlayerInfo.chr_type = CharacterType::_Tennis;
        break;

    case PointType::Soccer:
        p->m_PlayerInfo.chr_type = CharacterType::_Soccer;
        break;

    case PointType::BaseBall:
        p->m_PlayerInfo.chr_type = CharacterType::_Baseball;
        break;

    case PointType::AmericanFootBall:
        p->m_PlayerInfo.chr_type = CharacterType::_Americanfootball;
        break;
    }
}

void SelectPointBase::LeaveCursor(SelectCursor* p)
{
    for (
        std::list<SelectCursor*>::iterator it = m_OnCursorData.begin();
        it != m_OnCursorData.end();
        ++it)
    {
        if (*it == p)
        {
            m_OnCursorData.erase(it);
            return;
        }
    }

    MyAssert(false,"乗っていないカーソルが除去されました");
}

void SelectPointBase::Select(SelectCursor* p)
{
    bool PreFrameSelect = p->m_Selected;

    switch (m_Type)
    {

    case PointType::Tennis:
    case PointType::Soccer:
    case PointType::BaseBall:
    case PointType::AmericanFootBall:
        p->m_Lock = true;
        p->m_Selected = true;
        break;



    case PointType::Random:
        m_pMgr->RandomMove(p);
        break;



    case PointType::ComputerStrong:
        p->m_PlayerInfo.strong_type = GetChangeStrongType(p->m_PlayerInfo.strong_type);
        break;

    case PointType::ComputerChrSelect:
        if (p->m_PlayerInfo.player_type == PlayerType::_Player)
        {
            //p->m_Lock = true;
        }
        break;

    default:
        MyAssert(false, "タイプがおかしい");
        break;
    }

    //決定していたらＳＥを鳴らす
    if (PreFrameSelect == false && p->m_Selected)
    {
        Sound::Play(Sound::Scene_Enter);
    }
}

void SelectPointBase::Cancel(SelectCursor* p)
{
    if (p->m_Lock == false)
    {
        return;
    }

    bool PreFrameSelect = p->m_Selected;

    switch (m_Type)
    {

    case PointType::Tennis:
    case PointType::Soccer:
    case PointType::BaseBall:
    case PointType::AmericanFootBall:
        p->m_Lock = false;
        p->m_Selected = false;
        break;

    case PointType::ComputerChrSelect:
    {
        if ( p->m_PlayerInfo.player_type = PlayerType::_Player ) {
            p->m_Lock = false;
        }
    }
        
    default:
        return;
    }

    //SE
    Sound::Play(Sound::Cursor_cancel);
}

int SelectPointBase::GetOnCursorID(SelectCursor* p)
{
    int ret_Count = 1;

    for (auto& it:m_OnCursorData)
    {
        if (it == p)
        {
            return ret_Count;
        }

        ++ret_Count;
    }

    //失敗
    return -1;
}

bool SelectPointBase::Update()
{
    const float Speed = 0.2f;

    m_Pos += (m_MoveTargetPos - m_Pos)*Speed;

    return true;
}

bool SelectPointBase::Msg(MsgType mt)
{
    return false;
}

void SelectPointBase::CalcZ()
{
    m_SortZ = -2;
}

StrongType::Value SelectPointBase::GetChangeStrongType(StrongType::Value Now)
{
    switch (Now)
    {
    case StrongType::_Weak:
        return StrongType::_Usual;

    case StrongType::_Usual:
        return StrongType::_Strong;

    case StrongType::_Strong:
        return StrongType::_Weak;

    default:
        break;
    }

    return StrongType::_Weak;
}

void SelectPointBase::Render()
{
    if ( m_pTexture == nullptr )return;

    RectI di;

    di.x = (int)(m_Pos.x - m_Size.x*0.5f);
    di.y = (int)(m_Pos.y - m_Size.y*0.5f);

    di.w = (int)m_Size.x;
    di.h = (int)m_Size.y;

        m_pTexture->Render(
            di,
            m_TexRect
            );
    
}

//--------------------------------------------------------------------
//                  カーソルクラス
//--------------------------------------------------------------------

SelectCursor::SelectCursor(
    CursorManager*                  pMgr,
    controller::CONTROLLER_NUM      Num,
    LPIEX2DOBJ                      pTexture,
    const RectI&                    TexRect,
    SelectPointBase*                pInitPoint
    ) :
    m_pMgr(pMgr),
    m_ControllerNum(Num),
    m_pTexture(pTexture),
    m_TexRect(TexRect),
    m_pPoint(pInitPoint),
    m_Selected(false)
{
    switch (Num)
    {
    case 0:
        m_Pos = Vector2(-(float)iexSystem::ScreenWidth, -(float)iexSystem::ScreenHeight);
        break;

    case 1:
        m_Pos = Vector2((float)iexSystem::ScreenWidth * 2, -(float)iexSystem::ScreenHeight);
        break;

    case 2:
        m_Pos = Vector2(-(float)iexSystem::ScreenWidth, (float)iexSystem::ScreenHeight * 2);
        break;

    case 3:
        m_Pos = Vector2((float)iexSystem::ScreenWidth * 2, (float)iexSystem::ScreenHeight * 2);
        break;

    default:
        MyAssert(false, "対応カーソルＩＤを超えています");
        break;
    }

    m_pMgr->Regist(this);

    m_Lock = false;

    {
        m_PlayerInfo.number = (int)m_ControllerNum;
        m_PlayerInfo.chr_type = CharacterType::_Tennis;
        m_PlayerInfo.player_type = PlayerType::_Player;
        m_PlayerInfo.strong_type = StrongType::_Usual;
    }

    m_pPoint->OnCursor(this);
}

SelectCursor::~SelectCursor()
{

}

void SelectCursor::SetPoint(SelectPointBase* pNewPoint)
{
    m_pPoint->LeaveCursor(this);  //離れて

    m_pPoint = pNewPoint;  //更新

    m_pPoint->OnCursor(this); //乗る

    //SE
    Sound::Play(Sound::Cursor_move);
}

bool SelectCursor::SelectNowPoint()
{
    if (m_pPoint == nullptr )
    {
        return false;
    }

    m_pPoint->Select(this);
    return true;
}

void SelectCursor::Controll()
{
    {
        const bool StickDown = controller::GetStickJustMove(controller::stick::left, m_ControllerNum);
        SelectPointBase* pNextPoint = nullptr;

        Vector2 Vec = controller::GetStickValue(controller::stick::left, m_ControllerNum);
        Vec.y = -Vec.y;

        if (StickDown &&
            m_pMgr->GetNextPoint(Vec, m_pPoint, &pNextPoint)
            )
        {
            //移動成功
            SetPoint(pNextPoint);
        }
    }

    if (controller::GetTRG(controller::button::maru, m_ControllerNum))
    {
        m_pPoint->Select(this);
    }
}

bool SelectCursor::Update()
{

    //操作更新
    if (m_ControllerNum != controller::__ERROR_CONTROLLER_NUM)
    {
        if (m_Lock == false)
        {
            Controll();
        }
        else
        {
            if (controller::GetTRG(controller::button::batu, m_ControllerNum))
            {
                m_pPoint->Cancel(this);
            }
        }
    }
    

    //移動
    if (m_pPoint != nullptr)
    {
        m_Pos += (m_pPoint->GetOffSetPos(this) - m_Pos) * 0.2f;
    }

    return true;
}

bool SelectCursor::Msg(MsgType mt)
{
    return false;
}

void SelectCursor::CalcZ()
{
    m_SortZ = -4;
}

void SelectCursor::Render()
{
    RectI di;
    const Vector2 Size(150, 175);

    di.x = (int)(m_Pos.x - Size.x*0.5f);
    di.y = (int)(m_Pos.y - Size.y*0.5f);

    di.w = (int)Size.x;
    di.h = (int)Size.y;

    m_pTexture->Render(
        di,
        m_TexRect
        );
}

//--------------------------------------------------------------------
//                  カーソルマネージャ
//--------------------------------------------------------------------

CursorManager::CursorManager()
{

}

CursorManager::~CursorManager()
{

}

void CursorManager::Regist(SelectPointBase* p)
{
    m_PointData.push_back(p);
}

void CursorManager::Erace(SelectPointBase* p)
{
    for (auto& it : m_PointData)
    {
        if (it == p)
        {
            it = nullptr;
            break;
        }
    }
}

void CursorManager::Regist(SelectCursor* p)
{
    m_CursorData.push_back(p);
}

void CursorManager::Erace(SelectCursor* p)
{
    for (auto& it : m_CursorData)
    {
        if (it == p)
        {
            it = nullptr;
            break;
        }
    }
}

bool CursorManager::GetNextPoint(
    CrVector2           Vec,
    SelectPointBase*    pNow,
    SelectPointBase**   ppOut)
{
    const RADIAN OkAngle = D3DXToRadian(45);  //検知する角度
    RADIAN angle;
    float  MostNear = 1000000.0f, TempLen;
    Vector2 v;
    
    *ppOut = nullptr;
    
    for (auto& it : m_PointData)
    {
        if (it == nullptr || it == pNow || it->CanOnCursor() == false)
        {
            continue;
        }
    
        v = it->m_Pos - pNow->m_Pos;
    
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

bool CursorManager::isAllPlayerSelected()const
{
    for (auto& it : m_CursorData)
    {
        if ((it->m_PlayerInfo.player_type == PlayerType::_Player) && (it->m_Selected == false))
        {
            return false;
        }
    }

    return true;
}

bool CursorManager::isSelected(int player_number)const {

    int count = 0;
    for ( auto& it : m_CursorData ) {
        if ( count == player_number ) {
            return it->m_Selected;
        }
        count++;
    }
    return false;
}

void CursorManager::GetData(sceneGamePlay::InitParams& OutData)
{
    for (auto& it : m_CursorData)
    {
        sceneGamePlay::InitParams::PlayerInfo& p = it->m_PlayerInfo;

        if (p.number >= 0)
        {
            OutData.PlayerArray.at(p.number) = p;
        }
    }
    
}

void CursorManager::RandomMove(SelectCursor* p)
{
    class RandomMove :public GameObjectBase
    {
        SelectCursor* const         m_pMoveCursor;
        int                         m_Timer;
    public:

        std::list<SelectPointBase*> m_PointData;

        RandomMove(SelectCursor* p):
            m_pMoveCursor(p),
            m_Timer(0)
        {

        }

        SelectPointBase* GetRandomPoint()
        {
            SelectPointBase* pRet = nullptr;

            do
            {
                int r = rand() % (int)m_PointData.size();
                int cnt = 0;

                for (auto& it : m_PointData)
                {
                    if (cnt == r)
                    {
                        pRet = it;
                        break;
                    }
                    ++cnt;
                }
            } 
            while (pRet == m_pMoveCursor->GetPoint());

            return pRet;
        }

        bool Update()
        {
            bool isLive = m_Timer < 60;

            if (m_Timer++ % 5 == 0)
            {
                m_pMoveCursor->SetPoint(GetRandomPoint());
            }

            if (isLive == false)
            {
                m_pMoveCursor->SelectNowPoint();
            }

            return isLive;
        }

        bool Msg(MsgType mt)
        {
            return false;
        }
    };


    //ランダムな動きを！！
    RandomMove* r = new RandomMove(p);

    for (auto& it : m_PointData)
    {
        switch (it->m_Type)
        {
        case SelectPointBase::PointType::Tennis:
        case SelectPointBase::PointType::Soccer:
        case SelectPointBase::PointType::BaseBall:
        case SelectPointBase::PointType::AmericanFootBall:
            r->m_PointData.push_back(it);
            break;

        default:
            break;
        }
    }

    p->m_Lock = true;
}