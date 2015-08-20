#pragma once 

#include "../GameSystem/GameObject.h"

//------------------------------------------------------------------------
//  �����̃|�C���^���Q�[���I������delete���邾���̃Q�[���I�u�W�F�N�g
//------------------------------------------------------------------------

template <class ptr_Type>
class StaticGameObjectTemplate:public GameObjectBase
{
private:
    ptr_Type* const  Ptr;
public:

    StaticGameObjectTemplate(ptr_Type* ptr) :
        Ptr(ptr)
    {

    }

    ~StaticGameObjectTemplate()
    {
        delete Ptr;
    }

    bool Update()
    {
        return true;
    }

    bool Msg(MsgType mt)
    {
        return false;
    }
};