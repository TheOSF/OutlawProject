#ifndef __FADE_GAME_OBJECT_H__
#define __FADE_GAME_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "../utillity/ColorUtility.h"
#include "../Render/Renderer.h"

//-----------------------------------------------------------
// 　フェードアウトしてフェードインするゲームオブジェクト
//-----------------------------------------------------------

class FadeGameObject :public GameObjectBase
{
public:

    FadeGameObject(
        COLORf Color,        //色
        UINT   ChangeFrame,  //第一引数の色に変わるまでのフレーム
        UINT   FreezeFrame,  //色が変わってからとまるフレーム
        UINT   BackFrame     //元に戻るフレーム
        );

    ~FadeGameObject();

    bool Update();
    bool Msg(MsgType mt);

private:

    class Renderer :public ForwardRenderer
    {
    public:
        COLORf m_Color;

        Renderer();
        ~Renderer();

    private:
        void CalcZ();
        void Render();
    };

    const COLORf m_Color;        //色
    const UINT   m_ChangeFrame;  //変わるまでのフレーム
    const UINT   m_FreezeFrame;    //色が変わってからとまるフレーム
    const UINT   m_BackFrame;    //元に戻るフレーム
    UINT         m_Count;        //経過時間カウント
    Renderer     m_Renderer;     //矩形描画クラス
};

#endif