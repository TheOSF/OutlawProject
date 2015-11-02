#pragma once

#include "../GameSystem/GameObject.h"
#include "StagePhysicMoveObject.h"

//-------------------------------------------------//
//   ステージエディット　クラス
//-------------------------------------------------//

class StageEditer :public GameObjectBase
{
public:

    StageEditer();
    ~StageEditer();

    bool Update();
    bool Msg(MsgType mt);

    bool LoadAndEdit(const char* FilePath);
    
    static bool Load(const char* FilePath);
private:
    struct SetObjInfo
    {
        int type;
        Vector3 pos, angle;
    };

    typedef std::list<SetObjInfo> SetObjList;

    StagePhysicMoveObject*(*m_pCreateFunc)(CrVector3 pos, CrVector3 angle);
   
    StagePhysicMoveObject* m_pDrawObject;
    Vector3                m_Setpos, m_SetAngle;
    int                    m_SetFuncIndex;
    bool                   m_Enable;
    SetObjList             m_SetObjList;

    void Write(const char* file_path);
    void CursorControll();
    void Switch(bool plus);
    
};
