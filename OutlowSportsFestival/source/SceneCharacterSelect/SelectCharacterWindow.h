#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "../Render/MeshRenderer.h"
#include "../character/CharacterRenderer.h"
#include "CursorObject.h"
#include "../character/Baseball/BaseballEquip.h"
#include <array>

class SceneCharacterSelectUI;

//---------------------------------------------------------------//
// キャラクタセレクト時のウィンドウクラス
//---------------------------------------------------------------//

class SelectCharacterWindow :public GameObjectBase, public UserInterfaceRenderer 
{
public:
    SelectCharacterWindow(
        SelectCursor*   pCursor,
        iex2DObj*       pSelectTex
        );

    ~SelectCharacterWindow();

private:

    std::array<CharacterRenderer*, 4>   m_ChrRenderers;
    SelectCursor* const                 m_pCursor;
    MeshRenderer*                       m_pRandomMesh;
    MeshRenderer*                       m_pComMesh;
    iex2DObj* const                     m_pSelectTex;
    float                               m_3DMeshMoveCount;
    RATIO                               m_SelectedCount;

    //描画OnOff切り替え
    void SelectingRenderer(SelectPointBase::PointType type);
    
    //更新
    void ModelUpdate(SelectPointBase::PointType type);

    Matrix Calc3DMeshMatrix();

    void LoadCharacterModels();
    Vector3 GetCharacterPos();
    Vector2 GetUIPos();

    bool Update()override;
    bool Msg(MsgType mt)override;

    void CalcZ()override;
    void Render()override;
};