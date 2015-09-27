#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//----------------------------------------------------------------//
// パーティクルマネージャの初期化・更新・開放・描画を行うクラス
//----------------------------------------------------------------//

class ParticleManagerUpdater :public GameObjectBase, public ForwardRenderer
{
public:
    ParticleManagerUpdater();
    ~ParticleManagerUpdater();

private:
    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};