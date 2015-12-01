#include "StageEdit.h"
#include "StageObjectFactory.h"
#include "../GameSystem/GameController.h"
#include "IEX_Input.h"


static StagePhysicMoveObject*(*CreateFunc[])(CrVector3 pos, CrVector3 angle) =
{
    &StageObjFactory::CreateCone,
    &StageObjFactory::CreateBench,
    &StageObjFactory::CreatePipe,
};



StageEditer::StageEditer() :
m_pCreateFunc(&StageObjFactory::CreateCone),
m_SetFuncIndex(0),
m_Setpos(Vector3Zero),
m_SetAngle(Vector3Zero),
m_pDrawObject(nullptr),
m_Enable(false)
{

}

StageEditer::~StageEditer()
{

}



bool StageEditer::Update()
{
    if (KEY(KEY_ENTER, 0) == 3)
    {
        m_Enable = !m_Enable;

        if (m_Enable)
        {
            m_Setpos = Vector3(0, 5, 0);
            m_SetAngle = Vector3Zero;
        }
        else
        {
            if (m_pDrawObject != nullptr)
            {
                m_pDrawObject->m_Destroy = true;
                m_pDrawObject->m_Destroy = nullptr;
            }
        }
    }

    if (m_Enable == false)
    {
        return true;
    }

    //カーソルコントロール
    CursorControll();


    //１フレーム前の描画オブジェクトを消去
    if (m_pDrawObject != nullptr)
    {
        m_pDrawObject->m_Destroy = true;
    }


    //自分以外とめる
    {
        std::list<LpGameObjectBase> List;

        List.push_back(this);

        if (m_pDrawObject != nullptr)
        {
            List.push_back(m_pDrawObject);
        }

        DefGameObjMgr.FreezeOtherObjectUpdate(
            List,
            1
            );
    }


    //選択しているオブジェクトを描画
    m_pDrawObject = (*m_pCreateFunc)(m_Setpos, m_SetAngle);




    //書き込み
    if (KEY(KEY_C, 0) == 3)
    {
        Write("DATA\\Stages\\Stage1\\StageObjData.txt");
    }
    

    return true;
}

bool StageEditer::Msg(MsgType mt)
{

    return false;
}



bool StageEditer::LoadAndEdit(const char* file_path)
{
    FILE* fp;
    Vector3 pos, angle;
    int id = 0;
    
    fopen_s(&fp, file_path, "r");

    if (fp == 0)
    {
        fclose(fp);
        return false;
    }


    for (;;)
    {
        fscanf(fp, "%d", &id);

        if (id == -1)
        {
            break;
        }

        fscanf(fp,
            "%f %f %f %f %f %f \n",
            &pos.x,
            &pos.y,
            &pos.z,

            &angle.x,
            &angle.y,
            &angle.z
            );

        CreateFunc[id](pos, angle);

        m_SetObjList.push_back({ id, pos, angle });
    }

    fclose(fp);

    return true;
}

bool StageEditer::Load(const char* FilePath)
{
    FILE* fp;
    Vector3 pos, angle;
    int id = 0;


    if (fopen_s(&fp, FilePath, "r") != 0)
    {
        fclose(fp);
        return false;
    }

    for (;;)
    {
        fscanf(fp, "%d", &id);

        if (id == -1)
        {
            break;
        }

        fscanf(fp,
            "%f %f %f %f %f %f \n",
            &pos.x,
            &pos.y,
            &pos.z,

            &angle.x,
            &angle.y,
            &angle.z
            );

        CreateFunc[id](pos, angle);
    }

    fclose(fp);

    return true;
}

void StageEditer::Write(const char* file_path)
{
    FILE* fp;

    fopen_s(&fp, file_path, "wt");

    for (auto& it : m_SetObjList)
    {
        fprintf(fp, 
            "%d %f %f %f %f %f %f \n", 
            it.type,
            it.pos.x,
            it.pos.y,
            it.pos.z,

            it.angle.x,
            it.angle.y,
            it.angle.z
            );
    }

    fprintf(fp, "%d", -1);

    fclose(fp);
}

void StageEditer::CursorControll()
{
    float Speed = 0.1f;
    Vector3 v(0, 0, 0);

    float Key = KEY(KEY_AXISX2, 0)*0.001f*Speed;

    if (GetKeyState('X') & 0x80)v.x += Key;
    if (GetKeyState('Y') & 0x80)v.y += Key;
    if (GetKeyState('Z') & 0x80)v.z += Key;

    if (GetKeyState('W'))
    {
        m_Setpos += v;
    }
    else
    {
        v *= 0.1f;
        m_SetAngle += v;
    }

    if (controller::GetTRG(controller::button::_R1, 0))
    {
        Switch(true);
    }
    if (controller::GetTRG(controller::button::_L1, 0))
    {
        Switch(false);
    }


    if (KEY(KEY_SPACE, 0) == 3)
    {
        (*m_pCreateFunc)(m_Setpos, m_SetAngle);

        m_SetObjList.push_back({ m_SetFuncIndex, m_Setpos, m_SetAngle });
    }
}

void StageEditer::Switch(bool plus)
{

    if (plus)
    {
        m_SetFuncIndex = (m_SetFuncIndex + 1) % (int)ARRAYSIZE(CreateFunc);
    }
    else
    {
        --m_SetFuncIndex;
        if (m_SetFuncIndex < 0)
        {
            m_SetFuncIndex = (int)ARRAYSIZE(CreateFunc) - 1;
        }
    }

    m_pCreateFunc = CreateFunc[m_SetFuncIndex];
    
}