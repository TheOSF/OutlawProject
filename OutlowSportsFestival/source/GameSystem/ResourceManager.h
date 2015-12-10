#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

//-----------------------------------------------
//  �@�@�@�@���\�[�X�Ǘ��N���X
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

        //�X�e�[�W�P
        //Stage1_Hit,
        Stage1_HitPhysic,
        Bus,
        Car1,

        River,

        //�X�e�[�W�Q
        //Stage2_Hit,
        Stage2_HitPhysic,

        //����
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

    //--�o�^�����f�[�^�̃|�C���^���Q�b�g
    LPIEXMESH   Get(MeshType type);
    LPIEX2DOBJ  Get(TextureType type);

    //--�o�^����(�|�C���^��o�^���A������delete����)
    void Regist(MeshType type, LPIEXMESH p); 
    void Regist(TextureType type, LPIEX2DOBJ p);

    //--�폜����(�o�^����Ă���f�[�^���N���A)
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