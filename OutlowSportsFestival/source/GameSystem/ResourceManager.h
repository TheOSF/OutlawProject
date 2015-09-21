#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//-----------------------------------------------
//  　　　　リソース管理クラス
//-----------------------------------------------

class Resource
{
public:
    enum class MeshType
    {
        Sphere,
        HitEffect,

        Amefoot_ball,
        BaseBall_ball,
        Lacrosse_ball,
        Soccer_ball,
        Tennis_ball,
        Volley_ball,

        _MaxType
    };

    enum class TextureType
    {
        White,
        Particle,
        Locus1,
        Locus2,

        Anime_Circle,
        Anime_Smoke,
        Anime_Counter,

        _MaxType
    };

    static Resource& GetInstance();
    static void Release();

    //--登録したデータのポインタをゲット
    LPIEXMESH   Get(MeshType type);
    LPIEX2DOBJ  Get(TextureType type);

    //--登録する(ポインタを登録し、自動でdeleteする)
    void Regist(MeshType type, LPIEXMESH p); 
    void Regist(TextureType type, LPIEX2DOBJ p);

private:

    static Resource* m_pInstance;

    LPIEXMESH  m_pMeshArray[(int)MeshType::_MaxType];
    LPIEX2DOBJ m_pTextureArray[(int)TextureType::_MaxType];
    
    Resource();
    ~Resource();

    void Clear();
};

#define DefResource (Resource::GetInstance())

#endif