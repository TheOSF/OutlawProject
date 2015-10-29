#pragma once

#include "../Render/Renderer.h"
#include "iextreme.h"
#include "../utillity/ColorUtility.h"
#include "../GameSystem/GameObject.h"

//---------------------------------------------
//　板ポリゴンアニメーション描画クラス
//---------------------------------------------

class AnimationBordRenderer :protected ForwardHDRRenderer
{
public:

    AnimationBordRenderer(
        LPIEX2DOBJ      pTexture, //テクスチャ
        int             NumCellX, //アニメーションコマ数横
        int             NumCellY, //アニメーションコマ数縦
        int             MaxCell,  //総コマ数
        COLORf          Color,    //色
        DWORD           hdr       //HDRcolor
        );

    ~AnimationBordRenderer();

    int GetMaxCell()const;

    Vector3   m_Pos;          //板の中心座標
    Vector3   m_Right, m_Up;  //板の右方向と、向く方向
    Vector2   m_Size;         //板の大きさ

    int       m_CellCount;    //板にはるテクスチャのコマ番号

private:

    LPIEX2DOBJ      m_pTexture;
    int             m_NumCellX;
    int             m_NumCellY;
    int             m_MaxCell;
    DWORD           m_Color;
    DWORD           m_HdrCol;

    void CalcZ() override;
    void Render()override;
};



//---------------------------------------------------------
//　板ポリゴンアニメーション描画クラスを動かすクラス
//---------------------------------------------------------

class AnimationBordGameObj :public GameObjectBase
{
public:
    static const int m_Infinite_Livetime = INT_MAX;   //生存フレームを無限に指定する場合の定数


    Vector3 move_speed;              //座標の移動スピード(ワールド空間)
    Vector3 move_power;              //継続的にかかる力(ワールド空間)
                                     
    Vector3 rotate_speed;            //回転速度(ローカル空間)
    Vector3 rotate_power;            //継続的にかかる回転速度(ローカル空間)
                                     
    Vector2 scale_speed;             //大きくなるスピード(ローカル空間)
    Vector2 scale_power;             //継続的にかかる大きくなるスピード(ローカル空間)
                                     
    float   animation_speed;         //アニメーションのスピード(１で１フレームあたり１コマ進む)

    bool    animation_end_delete;    //アニメーションが終了している場合に消去するかどうか
    bool    animation_loop;          //アニメーションをループさせるかどうか


    AnimationBordGameObj(
        AnimationBordRenderer*   pRenderer,        //描画クラスへのポインタ(終了時にdeleteする)
        int     live_frame = m_Infinite_Livetime   //生存時間(デフォルトでは無限)
        );

    ~AnimationBordGameObj();

private:

    AnimationBordRenderer* const m_pRenderer;
    int                          m_LiveFrame;
    float                        m_MyCellCount;

    bool Update();
    bool Msg(MsgType mt);

    void UpdateFrame();
    void UpdateRotation();
    void UpdateScale();
    void UpdatePosition();
};