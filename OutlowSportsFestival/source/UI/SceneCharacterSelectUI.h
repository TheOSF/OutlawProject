#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


class SceneCharacterSelectUI : public UserInterfaceRenderer, public GameObjectBase {

public:
    SceneCharacterSelectUI(iex2DObj* const pTexture, const RectI& TextureRect);
    ~SceneCharacterSelectUI();

    struct Param {
        Vector2 Pos; // ���S����Ƃ������W
        Vector2 Size; // x : ���Ay : ����
        Vector4 Color;

        Param();
    };
    
    Param m_Current; // ���ݒl
    Param m_Target; // �ڕW�l

private:
    void PositionUpdate();

    void SizeUpdate();

    void ColorUpdate();

    bool Update()override;

    void Render()override;

    void CalcZ()override;

    bool Msg(MsgType mt)override {
        return false;
    }

private:
    iex2DObj* const m_pTexture; // �e�N�X�`��
    const RectI m_TextureRect; // �e�N�X�`���̎g�p�͈�

};