#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"
#include <vector>

//--------------------------------------------------------//
// 　簡易メニュー作成ＵＩクラス
//--------------------------------------------------------//

class SelectMenuUI:public UserInterfaceRenderer, public GameObjectBase
{
public:

    //カーソル移動方向の列挙
    enum class MoveVec
    {
        Up,
        Down,

        _NoMove
    };

    //操作クラスインターフェース
    class IController
    {
    public:
        virtual ~IController(){}

        virtual SelectMenuUI::MoveVec GetVec() = 0;  //移動方向を返さなければならない
        virtual bool isDecide() = 0;                 //戻り値がtrueなら決定
    };

    //決定した際に呼び出されるクラスのインターフェース
    class IDecideEvent
    {
    public:
        virtual ~IDecideEvent(){}
        virtual void Decide() = 0;
    };

    SelectMenuUI(
        IController*    pController,     //コントローラ(メニュー操作)クラスへのポインタ
        iex2DObj*       pBaseTexture,    //メニュー表示項目を参照するテクスチャ
        bool            isTextureDelete, //テクスチャをDeleteするかどうか
        float           Z                //奥行き(描画順番
        );

    ~SelectMenuUI();

    //項目を追加
    void AddMenu(
        IDecideEvent*   pDecideEvent,   //決定した際に呼び出される
        RectI           TextureRect,    //表示するテクスチャの矩形
        UINT            SizeX,          //表示する大きさＸ
        UINT            SizeY           //表示する大きさＹ
        );

private:


    //１項目の情報
    class MenuParam
    {
    public:
        MenuParam();
        ~MenuParam();

        IDecideEvent*   pDecideEvent;   //決定した際に呼び出される
        RectI           TextureRect;    //表示するテクスチャの矩形
        RATIO           SelectRatio;    //選択してからのタイマー(表示処理用)

        UINT            SizeX;          //表示する大きさＸ
        UINT            SizeY;          //表示する大きさＹ


        void SetSize(UINT X, UINT Y);
        void MulRect(RectI* pOut)const;  //拡大処理を行ったものを返す

    };


    typedef std::vector<MenuParam> MenuParamList;

    MenuParamList       m_MenuParamList;    //メニューリスト
    int                 m_NowSelectNum;     //現在選択している項目の番号

    IController* const  m_pController;      //コントローラ(メニュー操作)クラスへのポインタ
    iex2DObj* const     m_pBaseTexture;     //メニュー表示項目を参照するテクスチャ
    const bool          m_isTextureDelete;  //テクスチャをDeleteするかどうか
    const float         m_Z;                //奥行き(描画順番

    RATIO               m_Alpha;            //UIの透明度
    RATIO               m_BackBlack;        //UI背景の黒い度合い

    void(SelectMenuUI::*m_pStateFunc)();    //現在の実行する関数へのポインタ


    //------------------------------------//
    void State_FadeIn();
    void State_FadeOut();
    void State_CanControll();
    void State_Delete();
    //------------------------------------//


    void CalcZ();
    //描画(自動的に呼ばれる)
    void Render();

    void CalcRenderStartPos(int* OutX, int* OutY)const;   //描画開始位置を算出する
    int  CalcCursorNum(MoveVec vec)const;                 //移動後のカーソル番号を算出する
    void SelectUpdate(bool NoSelect);                     //メニュー項目の更新

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt)
    {
        return false;
    }

};