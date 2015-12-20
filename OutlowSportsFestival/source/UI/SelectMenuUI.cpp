#include "SelectMenuUI.h"

//--------------------------------------------------------//
// 　簡易メニュー作成ＵＩクラスのメニュー構造体
//--------------------------------------------------------//

SelectMenuUI::MenuParam::MenuParam()
{
    pDecideEvent = nullptr;

    {
        TextureRect.x = 0;
        TextureRect.y = 0;
        TextureRect.w = 500;
        TextureRect.h = 100;
    }

    SizeX = 500;
    SizeY = 100;

    SelectRatio = 0;
}

SelectMenuUI::MenuParam::~MenuParam()
{

}

void SelectMenuUI::MenuParam::SetSize(UINT X, UINT Y)
{
    SizeX = X;
    SizeY = Y;
}

void SelectMenuUI::MenuParam::MulRect(RectI* pOut)const  //拡大処理を行ったものを返す
{
    const int W = (int)((float)pOut->w * SelectRatio*0.2f);
    const int H = (int)((float)pOut->h * SelectRatio*0.2f);

    pOut->w += W;
    pOut->h += H;
    pOut->x -= W / 2;
    pOut->y -= H / 2;
}


//--------------------------------------------------------//
// 　簡易メニュー作成ＵＩクラス
//--------------------------------------------------------//

SelectMenuUI::SelectMenuUI(
    IController*    pController,     //コントローラ(メニュー操作)クラスへのポインタ
    iex2DObj*       pBaseTexture,    //メニュー表示項目を参照するテクスチャ
    bool            isTextureDelete, //テクスチャをDeleteするかどうか
    float           Z                //奥行き(描画順番
    ) :
    m_NowSelectNum(0),
    m_pController(pController),
    m_pBaseTexture(pBaseTexture),
    m_isTextureDelete(isTextureDelete),
    m_Z(Z),
    m_Alpha(0),
    m_BackBlack(0),
    m_pStateFunc(&SelectMenuUI::State_FadeIn)
{

}

SelectMenuUI::~SelectMenuUI()
{
    delete m_pController;

    if (m_isTextureDelete)
    {
        delete m_pBaseTexture;
    }

    for (auto& it : m_MenuParamList)
    {
        delete it.pDecideEvent;
    }
}

//項目を追加
void SelectMenuUI::AddMenu(
    IDecideEvent*   pDecideEvent,   //決定した際に呼び出される
    RectI           TextureRect,    //表示するテクスチャの矩形
    UINT            SizeX,          //表示する大きさＸ
    UINT            SizeY           //表示する大きさＹ
    )
{
    //パラメータ設定
    MenuParam Param;

    Param.pDecideEvent = pDecideEvent;
    Param.TextureRect = TextureRect;
    Param.SetSize(SizeX, SizeY);

    Param.SelectRatio = 0;

    //追加
    m_MenuParamList.push_back(Param);
}


//------------------------------ステート関数------------------------------//

void SelectMenuUI::State_FadeIn()
{
    const float Speed = 0.05f;

    //不透明度を加算
    m_Alpha += Speed;
   
    //１以上でステート移行
    if (m_Alpha > 1)
    {
        m_Alpha = 1.0f;
        m_pStateFunc = &SelectMenuUI::State_CanControll;
    }

    //背景の黒は不透明度と動機
    m_BackBlack = m_Alpha * 0.75f;


}

void SelectMenuUI::State_FadeOut()
{
    const float Speed = 0.05f;

    //不透明度を減算
    m_Alpha -= Speed;

    //０以下でステート移行
    if (m_Alpha < 0)
    {
        m_Alpha = 1.0f;
        m_pStateFunc = &SelectMenuUI::State_Delete;
    }

    //背景の黒は不透明度と同じ同じ
    m_BackBlack = m_Alpha;

    //選択更新
    SelectUpdate(false);
}

void SelectMenuUI::State_CanControll()
{

    //メニュー項目が無かったら終了
    if (m_MenuParamList.empty())
    {
        return;
    }

    //移動方向
    const MoveVec Vec = m_pController->GetVec();

    //カーソル位置を更新
    m_NowSelectNum = CalcCursorNum(Vec);

    //決定かどうか
    if (m_pController->isDecide())
    {
        //決定処理
        m_MenuParamList.at(m_NowSelectNum).pDecideEvent->Decide();
        
        //フェードアウトステートへ
        m_pStateFunc = &SelectMenuUI::State_FadeOut;
    }

    //選択更新
    SelectUpdate(false);
}

void SelectMenuUI::State_Delete()
{
    
}

//--------------------------------------------------------------------------//


void SelectMenuUI::CalcZ()
{
    m_SortZ = m_Z;
}

//描画(自動的に呼ばれる)
void SelectMenuUI::Render()
{
    //項目が無かったら終了
    if (m_MenuParamList.empty())
    {
        return;
    }

    //UI背景を黒く描画
    iexPolygon::Rect(
        0,
        0,
        iexSystem::ScreenWidth,
        iexSystem::ScreenHeight,
        RS_COPY,
        D3DCOLOR_COLORVALUE(0, 0, 0, m_BackBlack)
        );

    //項目を各描画
    {
        RectI DrawRect, TempRect;
        float Color = 0;

        //描画開始位置を算出
        CalcRenderStartPos(&DrawRect.x, &DrawRect.y);

        //描画
        for (auto &it : m_MenuParamList)
        {

            TempRect = DrawRect;

            //描画サイズの取得
            {
                TempRect.w = (int)it.SizeX;
                TempRect.h = (int)it.SizeY;

                it.MulRect(&TempRect);
            }

            //色の度合いを計算
            Color = fClamp(0.5f + it.SelectRatio*0.5f, 1, 0);

            //描画
            m_pBaseTexture->Render(
                TempRect,
                it.TextureRect,
                RS_COPY,
                D3DCOLOR_COLORVALUE(Color, Color, Color, m_Alpha)
                );

            //描画した縦の大きさの分移動する
            DrawRect.y += it.SizeY;
        }
    }

}

//描画開始位置を計算
void SelectMenuUI::CalcRenderStartPos(int* OutX, int* OutY)const
{
    //項目が無かったら終了
    if (m_MenuParamList.empty())
    {
        *OutX = 0;
        *OutY = 0;
        return;
    }

    int W = 0, H = 0;


    {
        for (auto &it : m_MenuParamList)
        {
            W = max(W, (int)it.SizeX);
            H += it.SizeY;
        }
    }

    W /= 2;
    H /= 2;

    *OutX = (int)iexSystem::ScreenWidth / 2 - W;
    *OutY = (int)iexSystem::ScreenHeight / 2 - H;
}

int  SelectMenuUI::CalcCursorNum(MoveVec vec)const
{   
    int CalcCursorNum = m_NowSelectNum;

    switch (vec)
    {
    case MoveVec::_NoMove:
        //移動していなかった場合は何もしない
        break;

    case MoveVec::Up:
        //上に移動
        --CalcCursorNum;
        break;

    case MoveVec::Down:
        //下に移動
        ++CalcCursorNum;
        break;
    }

    //範囲内に収める
    CalcCursorNum = max(CalcCursorNum, 0);
    CalcCursorNum = min(CalcCursorNum, (int)m_MenuParamList.size() - 1);

    return CalcCursorNum;
}

void SelectMenuUI::SelectUpdate(bool NoSelect)
{
    //メニュー項目の更新
    const int NumMenu = (int)m_MenuParamList.size();

    for (int i = 0; i < NumMenu; ++i)
    {
        auto& it = m_MenuParamList.at(i);

        //現在選択中なら
        if (NoSelect == false && i == m_NowSelectNum)
        {
            //選択度合いを上げる
            it.SelectRatio += (1.0f - it.SelectRatio)*0.2f;
        }
        else
        {
            //選択度合いを下げる
            it.SelectRatio *= 0.2f;
        }
    }

}

bool SelectMenuUI::Update()	//falseを返すと消去する
{
    //現在のステート関数を実行
    (this->*m_pStateFunc)();

    //終了ステートならfalseを返す
    return m_pStateFunc != &SelectMenuUI::State_Delete;
}
