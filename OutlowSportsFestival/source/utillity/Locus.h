#ifndef __LOCUS_H__
#define __LOCUS_H__

#include "iextreme.h"
#include "../Render/Renderer.h"

//----------------------------------------------
//    �O�Օ`��N���X
//----------------------------------------------

class Locus :protected ForwardRenderer
{
public:
	struct Param
	{
		float   Width;   //�т̕�
		Vector4 Color;   //�F
	};

	bool              m_Visible;         //�`�悷�邩�ǂ���

	LPIEX2DOBJ        m_pTexture;        //�тɒ���e�N�X�`��(null�ŐF�̂�)

	Param             m_StartParam;      //�J�n�n�_�̏��
	Param             m_EndParam;        //�I���n�_�̏��

	UINT              m_Division;        //��Ԃ̕�����

    DWORD             m_dwFlags;         //�`��X�e�[�g

	Locus(
		size_t SavePointNum  //�ۑ�����_�̐�
		);
	~Locus();

	//�т̓_��ǉ�����
	void AddPoint(
		CrVector3 pos,   //���S�ʒu
		CrVector3 vec    //�т̐L�т����(���K�����Ă��Ȃ���΂Ȃ�Ȃ�)
		);

	//�т̒������O��
	void Clear();

	//�O�Ղ̓_����납���������
	void SubPoint(
		size_t num       //��������_�̐�
		);

private:

	struct Point
	{
		Vector3 pos;
		Vector3 vec;
	};

	Point*		    m_pPointData;
	const size_t    m_NumPoint;
	size_t		    m_UseCount;

	void Initialize();
		
	void CalcZ();
	void Render();

	void RenderDivision();
	void RenderUsual();

	void Render(
		const Param& param1,
		const Param& param2,
		const Point& point1,
		const Point& point2,
		float t,
		float t2
		);

	void LeapParam(
		Param& out,
		const Param& in1,
		const Param& in2,
		float t
		);
};

#endif