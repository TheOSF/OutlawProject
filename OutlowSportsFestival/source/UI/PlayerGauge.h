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

        void AddRoundIcon(CrVector2 Pos);
        void UpdateRoundIcon();
        void SetShock(RATIO power,int frame);

    private:
        static const int RoundCountMax = 20;

        struct RoundIconParam
        {
            bool    Enable;
            int     Frame;
            float   Size;
            Vector2 Pos;
        };

        iex2DObj* m_pUItex;
        iex2DObj* m_pIconTex;

        std::array<RoundIconParam, RoundCountMax> m_RoundIconParams;

        void BackRender();
        void LifeRender();
        void SpRender();
        void FaceRender();
        void PlayerNumRender();
        void WinIconRender(CrVector2 pos,float Size);
        void AllWinIconRender();

        Vector2 CalcIconPos(int n);

        void CalcZ();
        void Render();
    };

    GaugeRenderer       m_Renderer;
    CharacterBase*      m_pOwnerCharacter;
    UINT                m_RoundCount;
    Vector2             m_DefaultPos;
    RATIO               m_ShockPower;
    float               m_PreFrameLife;

    bool Update();
    bool Msg(MsgType mt);
};

