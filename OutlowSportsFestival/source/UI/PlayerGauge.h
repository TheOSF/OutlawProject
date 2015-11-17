#pragma once


#include "iextreme.h"
#include "../character/CharacterBase.h"
#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"
#include "../utillity/ColorUtility.h"


//-----------------------------------------------
//  プレイヤーゲージクラス
//-----------------------------------------------

class PlayerGauge :public GameObjectBase
{
public:
    PlayerGauge(CharacterBase* pOwnerCharacter);
    ~PlayerGauge();

private:

    class GaugeRenderer :public UserInterfaceRenderer
    {
    public:

        GaugeRenderer();

        COLORf  m_Color;
        COLORf  m_FaceColor;
                
        RATIO   m_Life;
        RATIO   m_Sp;

        Vector2 m_Pos;
        Vector2 m_Size;

        PlayerNum::Value m_Num;

        PlayerType::Value m_PlayerType;

    private:

        iex2DObj* m_pUItex;
        iex2DObj* m_pIconTex;

        void BackRender();
        void LifeRender();
        void SpRender();
        void FaceRender();
        void PlayerNumRender();


        void CalcZ();
        void Render();
    };

    GaugeRenderer     m_Renderer;
    CharacterBase*    m_pOwnerCharacter;

    bool Update();
    bool Msg(MsgType mt);
};

