#pragma once

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "../Render/MeshRenderer.h"
#include "../character/CharacterRenderer.h"
#include "CursorObject.h"
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


private:

    std::array<CharacterRenderer*, 4>   m_ChrRenderers;
    SelectCursor* const                 m_pCursor;
    bool                                m_DrawComputerUI;
    MeshRenderer*                       m_pRandomMesh;

    void LoadCharacterModels();
    Vector3 GetCharacterPos();

    bool Update()override;
    bool Msg(MsgType mt)override;


    void CalcZ()override;
    void Render()override;
};