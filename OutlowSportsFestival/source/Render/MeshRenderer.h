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

	MeshRenderer(
		LPIEXMESH	pMesh,		//描画するメッシュのポインタ
		bool		MeshDelete,	//第一引数のメッシュをdeleteするかどうか
        RenderType  type
		);

	~MeshRenderer();

	//描画
    void GbufRender(
        iexShader*        pShader,                       //シェーダークラス
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
        );

    void MasterRender();

    void DepthRender(iexShader* pShader, const char* pTec);
    
	//ワールド変換行列のセッタ・ゲッタ
	void SetMatrix(const Matrix& mat);
	const Matrix& GetMatrix()const;

    //故意に輝かせる際のパラメータ(xyz = rgb)
    Vector3 m_HDR;
private:
    Matrix              m_TransMatrix;
	iexMesh*	const	m_pMesh;
	bool		const	m_MeshDelete;
    RenderType  const   m_RenderType;
};

#endif