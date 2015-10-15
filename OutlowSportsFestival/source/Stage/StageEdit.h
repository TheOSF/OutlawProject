#pragma once

#include "../GameSystem/GameObject.h"


//-------------------------------------------------//
//   �X�e�[�W�G�f�B�b�g�@�N���X
//-------------------------------------------------//

class StageEditer :public GameObjectBase
{
public:

    StageEditer();
    ~StageEditer();

    bool Update();
    bool Msg(MsgType mt);

    bool Load(const char* file_path);

private:

    Vector3  m_Setpos, m_SetAngle;

    void Write();
    
};

