#pragma once

//------------------------------------------------------------//
// 汎用ＵＩ描画クラス
//------------------------------------------------------------//

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"
#include "StateTemplate.h"

class UIbase :public UserInterfaceRenderer, public GameObjectBase
{
public:
    typedef State<UIbase, void> State;

    //描画タイプ列挙
    enum class DrawType
    {
        Usual,  //通常
        Add,    //加算
        Sub     //減算
    };

    UIbase();
    ~UIbase();

    void SetState(State* NewState);  //新しいステートをセット

    struct
    {
        bool      Delete;     //削除フラグ(trueでDeleteされる)
        iex2DObj* Texture;    //テクスチャへのポインタ
        RectI     UV;         //テクスチャのサンプリングする矩形座標
        Vector3   pos;        //座標(x = 画面座標　y = 画面座標　z = z＞０ 奥行き(描画順番) )
        Vector2   scale;      //スケール
        RADIAN    angle;      //アングル
        DWORD     color;      //色
        DrawType  dw_type;    //描画タイプ

    }param;

private:
    typedef StateMachine<UIbase, void> MyStateMacihne;

    MyStateMacihne* m_pStateMachine;

    void CalcZ();
    void Render();
    bool Update();
    bool Msg(MsgType mt);
};