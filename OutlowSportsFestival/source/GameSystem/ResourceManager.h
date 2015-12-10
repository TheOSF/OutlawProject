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
        Pole,

        HitEffect,

        Amefoot_ball,
        BaseBall_ball,
        Lacrosse_ball,
        Soccer_ball,
        Soccer_Special_Hit,

        Tennis_ball,
        Volley_ball,

        //ステージ１
        //Stage1_Hit,
        Stage1_HitPhysic,
        Bus,
        Car1,

        River,

        //ステージ２
        //Stage2_Hit,
        Stage2_HitPhysic,

        //共通
        Cone,
        Bench,
        Pipe,


        _MaxType
    };

    enum class TextureType
    {
        White,
        Particle,
        Locus1,
        Locus2,
        Konoha,

        UI_strings,
        UI_player_name,
        UI_icon,
        UI_gauges,

        Anime_Circle,
        Anime_Smoke,
        Anime_Counter,
		Anime_Change,
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

    //--削除する(登録されているデータをクリア)
    void Erace(MeshType type);
    void Erace(TextureType type);
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