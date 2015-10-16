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

        GaugeRenderer(
            CharacterBase* pOwnerCharacter
            );

        COLORf m_Color;
        COLORf m_FaceColor;

        RATIO  m_Life;
        RATIO  m_Sp;
        
    private:
        void LifeRender();
        void SpRender();
        void FaceRender();

        void CalcZ();
        void Render();
    };
    

    bool Update();
    bool Msg(MsgType mt);
};

