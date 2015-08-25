#ifndef __FORWARD_HDR_MESH_RENDERER_H__
#define __FORWARD_HDR_MESH_RENDERER_H__

#include "Renderer.h"

//------------------------------------------------
//  HDRメッシュレンダラー
//------------------------------------------------

class ForwardHDR_MeshRenderer :public ForwardHDRRenderer
{
public:

    ForwardHDR_MeshRenderer(
        LPIEXMESH	pMesh,		//描画するメッシュのポインタ
        bool		MeshDelete	//第一引数のメッシュをdeleteするかどうか
        );

    ~ForwardHDR_MeshRenderer();

    //ワールド変換行列のセッタ・ゲッタ
    void SetMatrix(const Matrix& mat);
    const Matrix& GetMatrix()const;

    Vector3 m_HDR_Color;

private:
    iexMesh*	const	m_pMesh;
    bool		const	m_MeshDelete;

    void CalcZ();
    void Render();
};

#endif