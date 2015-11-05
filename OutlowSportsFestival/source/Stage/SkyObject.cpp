#include "SkyObject.h"


SkyObject::SkyObject(const char* pModelPath) :
m_DeleteFlg(false)
{
    char Path[MAX_PATH];

    strcpy_s<MAX_PATH>(Path, pModelPath);

    m_pMesh = new iexMesh(Path);


    m_pMesh->SetPos(Vector3(0, 75, 200));
    m_pMesh->SetScale(200,100,0);
    m_pMesh->SetAngle(PI);
    m_pMesh->Update();
}



SkyObject::~SkyObject()
{
    delete m_pMesh;
    m_pMesh = nullptr;
}

bool SkyObject::Update()
{
    return !m_DeleteFlg;
}

bool SkyObject::Msg(MsgType mt)
{
    return false;
}

void SkyObject::CalcZ()
{
    //‚©‚È‚è‰œ‚Ì‚Ù‚¤‚É
    m_SortZ = 1000.0f;
}

void SkyObject::Render()
{
    m_pMesh->Render(
        shader,
        "Sky"
        );
}