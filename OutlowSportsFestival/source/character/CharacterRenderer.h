#ifndef __CHARACTER_RENDERER_H__
#define __CHARACTER_RENDERER_H__

#include "../Render/Renderer.h"
#include "../brendanimation/BlendAnimationMesh.h"
#include "../utillity/ColorUtility.h"
#include <vector>

//*************************************************
//	�L�����N�^�p���b�V���N���X
//*************************************************

class CharacterRenderer :public DeferredRenderer, public ForwardHDRRenderer
{
public:
    enum class RenderType
    {
        Skin,
        Hair,
        Clothes,
        Face,
        Normal,
    };

	//�ʏ�R���X�g���N�^
	CharacterRenderer(
		BlendAnimationMesh* pAnimeMesh
		);

	//���ʃ��[�V����������ۂ�
	CharacterRenderer(
		BlendAnimationMesh*		pAnimeMesh,
		const std::vector<int>& UpBodyBoneNums,
		const std::vector<int>& DownBodyBoneNums
		);

	~CharacterRenderer();


    //�}�e���A�����ǂ̕��ʂ����w�肷��(�`��؂�ւ��̂���)
    void SetMaterialRenderType(int MaterialNum, RenderType Type);


	//�ʏ�̃��[�V�����Z�b�g
	void SetMotion(int m);

	//�㔼�g�E�����g���Ƃ̃{�[�V�����Z�b�g
	void SetBodyUpMotion(int m);
	void SetBodyDownMotion(int m);

	//�ʏ�̃��[�V�����X�s�[�h�Z�b�g
	void SetMotionSpeed(float t);

	//�㔼�g�E�����g���Ƃ̃X�s�[�h�Z�b�g
	void SetBodyUpMotionSpeed(float t);
	void SetBodyDownMotionSpeed(float t);

	//���[�V�����X�V
	void Update(float t);

    //���[���h��ԏ�ł̃{�[���s��𓾂�
    void GetWorldBoneMatirx(Matrix& Out, int BoneNum);

    //���[�J�����(�L�����N�^���)��ł̃{�[���s��𓾂�
    void GetLocalBoneMatrix(Matrix& Out, int BoneNum);

    //���[���h��ԏ�ł̃{�[�����W�𓾂�
    Vector3 GetWorldBonePos(int BoneNum);

    //���[�J�����(�L�����N�^���)��ł̃{�[�����W�𓾂�
    Vector3 GetLocalBonePos(int BoneNum);

	//���[���h�ϊ��s��
	Matrix  m_TransMatrix;

    //�̑S�̂̋P���x(�O���W���Axyz = rgb)
    Vector3 m_HDR;

    //���C�g�l�����Z����
    Vector3 m_Lighting;

    //���̐F�ixyz = rgb)
    Vector3 m_ClothesColor;

    //�֊s����`�悷�邩�ǂ���
    bool    m_OutlineVisible;

    //�֊s���̐F(�P�ȏ�̒l�͌���A�Q���ő�l)
    COLORf m_OutlineColor;


    void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type);

private:
	typedef std::vector<int> BoneNumArray;
    typedef std::map<int, const char*> Techniques;

	BlendAnimationMesh* m_pAnimeMesh;
	const bool m_UsePartsMotion;

	const BoneNumArray m_UpBodyBoneNumArray;
	const BoneNumArray m_DownBodyBoneNumArray;

	int m_BodyUpMotion;
	int m_BodyDownMotion;

	float m_BodyUpMotionSpeed;
	float m_BodyDownMotionSpeed;

    Techniques m_Techniques;

	void Initialize();

    void GbufRender(
        iexShader*        pShader,                       //�V�F�[�_�[�N���X
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
        );

    void MasterRender();


    void CalcZ();
    void Render();
};

#endif