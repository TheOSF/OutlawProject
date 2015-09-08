#ifndef __FORWARD_HDR_MESH_RENDERER_H__
#define __FORWARD_HDR_MESH_RENDERER_H__


#include "Renderer.h"
#include "../utillity/ColorUtility.h"


//------------------------------------------------
//  HDR���b�V�������_���[
//------------------------------------------------

class ForwardHDR_MeshRenderer :public ForwardHDRRenderer
{
public:

    ForwardHDR_MeshRenderer(
        LPIEXMESH	pMesh,		//�`�悷�郁�b�V���̃|�C���^
        bool		MeshDelete	//�������̃��b�V����delete���邩�ǂ���
        );

    ~ForwardHDR_MeshRenderer();

    //���[���h�ϊ��s��̃Z�b�^�E�Q�b�^
    void SetMatrix(const Matrix& mat);
    const Matrix& GetMatrix()const;

    COLORf  m_Color;
    Vector3 m_HDR_Color;

private:
    Matrix              m_TransMatrix;
    iexMesh*	const	m_pMesh;
    bool		const	m_MeshDelete;

    void CalcZ();
    void Render();
};

#endif