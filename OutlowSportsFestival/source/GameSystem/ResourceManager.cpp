#include "ResourceManager.h"

Resource* Resource::m_pInstance = nullptr;


Resource& Resource::GetInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new Resource();
    }

    return *m_pInstance;
}


void Resource::Release()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

LPIEXMESH Resource::Get(MeshType type)
{
    MyAssert(m_pMeshArray[(int)type] != nullptr, "リソースが登録されていない状態でMesh::Getが呼ばれました type=%d", (int)type);

    return m_pMeshArray[(int)type];
}

LPIEX2DOBJ Resource::Get(TextureType type)
{
    MyAssert(m_pTextureArray[(int)type] != nullptr, "リソースが登録されていない状態でTexture::Getが呼ばれました type=%d", (int)type);

    return m_pTextureArray[(int)type];
}

void Resource::Regist(MeshType type, LPIEXMESH p)
{
    MyAssert(m_pMeshArray[(int)type] == nullptr, "リソースの二重登録が行われました type=%d", (int)type);

    m_pMeshArray[(int)type] = p;
}

void Resource::Regist(TextureType type, LPIEX2DOBJ p)
{
    MyAssert(m_pTextureArray[(int)type] == nullptr, "リソースの二重登録が行われました type=%d", (int)type);

    m_pTextureArray[(int)type] = p;
}

Resource::Resource()
{
    for (int i = 0; i < (int)MeshType::_MaxType; ++i)
    {
        m_pMeshArray[i] = nullptr;
    }

    for (int i = 0; i < (int)TextureType::_MaxType; ++i)
    {
        m_pTextureArray[i] = nullptr;
    }
}


Resource::~Resource()
{
    Clear();
}

void Resource::Clear()
{
    for (int i = 0; i < (int)MeshType::_MaxType; ++i)
    {
        delete m_pMeshArray[i];
       m_pMeshArray[i] = nullptr;
    }

    for (int i = 0; i < (int)TextureType::_MaxType; ++i)
    {
        delete m_pTextureArray[i];
        m_pTextureArray[i] = nullptr;
    }
}


