#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//-------------------------------------------------------//
//  空描画クラス
//-------------------------------------------------------//

class SkyObject :public GameObjectBase, public ForwardHDRRenderer
{
public:
    SkyObject(const char* pModelPath);
    ~SkyObject();

    bool m_DeleteFlg;

private:

    iexMesh* m_pMesh;

    bool Update();
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();

};