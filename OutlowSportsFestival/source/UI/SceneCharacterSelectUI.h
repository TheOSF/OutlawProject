#pragma once

#include "../Render/Renderer.h"
#include "../GameSystem/GameObject.h"


class SceneCharacterSelectUI : public UserInterfaceRenderer, public GameObjectBase {

public:
    SceneCharacterSelectUI(iex2DObj* const pTexture, const RectI& TextureRect);
    ~SceneCharacterSelectUI();

    struct Param {
        Vector2 Pos; // 中心を基準とした座標
        Vector2 Size; // x : 幅、y : 高さ
        Vector4 Color;

        Param();
    };
    
    Param m_Current; // 現在値
    Param m_Target; // 目標値

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
    iex2DObj* const m_pTexture; // テクスチャ
    const RectI m_TextureRect; // テクスチャの使用範囲

};