#pragma once

#include "../GameSystem/GameObject.h"
#include "../Render/Renderer.h"

//----------------------------------------------------------------//
// �p�[�e�B�N���}�l�[�W���̏������E�X�V�E�J���E�`����s���N���X
//----------------------------------------------------------------//

class ParticleManagerUpdater :public GameObjectBase, public ForwardRenderer
{
public:
    ParticleManagerUpdater();
    ~ParticleManagerUpdater();

private:
    bool Update();	//false��Ԃ��Ə�������
    bool Msg(MsgType mt);

    void CalcZ();
    void Render();
};