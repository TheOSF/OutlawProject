#pragma once

#include "../Render/Renderer.h"
#include "../character/CharacterBase.h"

class PlayerCursorBillbord :public ForwardRenderer
{
public:
    PlayerCursorBillbord(
        CharacterBase*  pCharacter
        );

    ~PlayerCursorBillbord();


private:
    
    const CharacterBase* const m_pCharacter;
    iex2DObj* const            m_pCursorTex;
    RectI                      m_TextureRect;
    RectI                      m_DrawRect;
    RATIO                      m_Alpha;
    
    void CalcZ()override;
    void Render()override;
};