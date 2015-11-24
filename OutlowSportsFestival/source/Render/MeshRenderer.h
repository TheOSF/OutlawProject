#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include "Renderer.h"
#include "iextreme.h"


//**************************************************************************
//		メッシュレンダラークラス(メッシュをマイフレーム自動的に描画する)
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
        LPIEXMESH	      pMesh,		//描画するメッシュのポインタ
        bool		      MeshDelete,	//第一引数のメッシュをdeleteするかどうか
        RenderType        render_Type,
        GbufRenderType    gbuf_Type = GbufRenderType::NoTexture,
        PreRenderCallBack* pCallBack = nullptr
		);

	~MeshRenderer();

	//描画
    void GbufRender(
        iexShader*        pShader,                       //シェーダークラス
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
        );

    void MasterRender();

    void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type);
    
	//ワールド変換行列のセッタ・ゲッタ
	void SetMatrix(const Matrix& mat);
	const Matrix& GetMatrix()const;

    iexMesh*		    m_pMesh;

    //故意に輝かせる際のパラメータ(xyz = rgb)
    Vector3 m_HDR;

    //故意に光らせる際のパラメータ(xyz = rgb)
    Vector3 m_Lighting;
private:
    Matrix               m_TransMatrix;
	bool		const	 m_MeshDelete;
    RenderType  const    m_RenderType;
    GbufRenderType       m_Gbuf_Type;
    PreRenderCallBack*   m_pCallBack;
};

#endif