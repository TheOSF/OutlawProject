#pragma once

#include "../Render/Renderer.h"
#include "../character/CharacterBase.h"
#include "../GameSystem/GameObject.h"

//-----------------------------------------------------------------//
//   �L�����N�^�̏�ɕ\������@�v���C���[�i���o�[�t�h�I�u�W�F�N�g
//-----------------------------------------------------------------//

class PlayerCursorBillbord :public UserInterfaceRenderer, public GameObjectBase
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
    
    bool Update()override;
    bool Msg(MsgType mt)override;

    void CalcZ()override;
    void Render()override;
};