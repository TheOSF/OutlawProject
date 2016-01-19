#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "../Render/MeshRenderer.h"
#include "../character/CharacterRenderer.h"
#include "CursorObject.h"
#include "../character/Baseball/BaseballEquip.h"
#include <array>

//---------------------------------------------------------------//
// キャラクタセレクト時のウィンドウクラス
//---------------------------------------------------------------//

class SelectCharacterWindow :public GameObjectBase, public UserInterfaceRenderer
{
public:
    SelectCharacterWindow(
        SelectCursor*   pCursor
        );

    ~SelectCharacterWindow();

    void DoMotion();

private:

    std::array<CharacterRenderer*, 4>   m_ChrRenderers;
    SelectCursor* const                 m_pCursor;
    bool                                m_DrawComputerUI;
    MeshRenderer*                       m_pRandomMesh;
    bool                                m_PreFrameIsSelected;
    BaseballEquip*                      m_pBaseballEquip;

    void SelectingRenderer(SelectPointBase::PointType type);

    void LoadCharacterModels();
    Vector3 GetCharacterPos();

    bool Update()override;
    bool Msg(MsgType mt)override;


    void CalcZ()override;
    void Render()override;
};