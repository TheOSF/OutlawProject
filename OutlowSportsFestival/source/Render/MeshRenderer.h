#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "Renderer.h"
#include "iextreme.h"


//**************************************************************************
//		���b�V�������_���[�N���X(���b�V�����}�C�t���[�������I�ɕ`�悷��)
//**************************************************************************

class MeshRenderer :public DeferredRenderer
{
public:
    enum class RenderType
    {
        NoTexture,
        UseColor,
        UseColorSpecular,
        UseColorSpecularLightMap,
    };

    enum class GbufRenderType
    {
        NoTexture,
        UseNormal,
        UseNormalHeight
    };

    class PreRenderCallBack
    {
    public:
        enum Type
        {
            MasterRender,
            GbufRender,
            DepthRender,
        };
        virtual ~PreRenderCallBack(){}
        virtual void Execute(MeshRenderer* pRenderer, Type type){}
    };

    MeshRenderer(
        LPIEXMESH	      pMesh,		//�`�悷�郁�b�V���̃|�C���^
        bool		      MeshDelete,	//�������̃��b�V����delete���邩�ǂ���
        RenderType        render_Type,
        GbufRenderType    gbuf_Type = GbufRenderType::NoTexture,
        PreRenderCallBack* pCallBack = nullptr
		);

	~MeshRenderer();

	//�`��
    void GbufRender(
        iexShader*        pShader,                       //�V�F�[�_�[�N���X
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
        );

    void MasterRender();

    void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type);
    
	//���[���h�ϊ��s��̃Z�b�^�E�Q�b�^
	void SetMatrix(const Matrix& mat);
	const Matrix& GetMatrix()const;

    iexMesh*		    m_pMesh;

    //�̈ӂɋP������ۂ̃p�����[�^(xyz = rgb)
    Vector3 m_HDR;

    //�̈ӂɌ��点��ۂ̃p�����[�^(xyz = rgb)
    Vector3 m_Lighting;
private:
    Matrix               m_TransMatrix;
	bool		const	 m_MeshDelete;
    RenderType  const    m_RenderType;
    GbufRenderType       m_Gbuf_Type;
    PreRenderCallBack*   m_pCallBack;
};

#endif