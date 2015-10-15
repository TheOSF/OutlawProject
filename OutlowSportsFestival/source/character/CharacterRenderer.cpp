#include "CharacterRenderer.h"
#include "../IexSystem/System.h"

//*************************************************
//	�L�����N�^�p���b�V���N���X
//*************************************************


//�ʏ�R���X�g���N�^
CharacterRenderer::CharacterRenderer(BlendAnimationMesh* pAnimeMesh) :
m_pAnimeMesh(pAnimeMesh),
m_UsePartsMotion(false),
m_HDR(0,0,0)
{
	Initialize();
}

//���ʃ��[�V����������ۂ�
CharacterRenderer::CharacterRenderer(
	BlendAnimationMesh*		pAnimeMesh,
	const std::vector<int>& UpBodyBoneNums,
	const std::vector<int>& DownBodyBoneNums
	):
	m_pAnimeMesh(pAnimeMesh),
	m_TransMatrix(pAnimeMesh->TransMatrix),
	m_UsePartsMotion(false),
	m_UpBodyBoneNumArray(UpBodyBoneNums),
	m_DownBodyBoneNumArray(DownBodyBoneNums)
{
	Initialize();
}

CharacterRenderer::~CharacterRenderer()
{
	delete m_pAnimeMesh;
}

//�ʏ�̃��[�V�����Z�b�g
void CharacterRenderer::SetMotion(int m)
{
	const int nb = m_pAnimeMesh->GetNumBone();

	for (int i = 0; i < nb; ++i)
	{
		m_pAnimeMesh->SetMotion(m, i, 0);
	}
}


//�㔼�g�E�����g���Ƃ̃{�[�V�����Z�b�g
void CharacterRenderer::SetBodyUpMotion(int m)
{
	m_BodyUpMotion = m;

	for (
		auto it = m_UpBodyBoneNumArray.begin();
		it != m_UpBodyBoneNumArray.end();
		++it
		)
	{
		m_pAnimeMesh->SetMotion(m, *it, 0);
	}
}

void CharacterRenderer::SetBodyDownMotion(int m)
{
	m_BodyDownMotion = m;

	for (
		auto it = m_DownBodyBoneNumArray.begin();
		it != m_DownBodyBoneNumArray.end();
	++it
		)
	{
		m_pAnimeMesh->SetMotion(m, *it, 0);
	}
}


//�ʏ�̃��[�V�����X�s�[�h�Z�b�g
void CharacterRenderer::SetMotionSpeed(float t)
{
	m_BodyUpMotionSpeed = m_BodyDownMotionSpeed = t;

	const int nb = m_pAnimeMesh->GetNumBone();

	for (int i = 0; i < nb; ++i)
	{
		m_pAnimeMesh->SetSpeed(i, 0, t);
	}
}

//�㔼�g�E�����g���Ƃ̃X�s�[�h�Z�b�g
void CharacterRenderer::SetBodyUpMotionSpeed(float t)
{
	m_BodyUpMotionSpeed = t;

	for (
		auto it = m_UpBodyBoneNumArray.begin();
		it != m_UpBodyBoneNumArray.end();
	++it
		)
	{
		m_pAnimeMesh->SetSpeed(*it, 0, t);
	}
}

void CharacterRenderer::SetBodyDownMotionSpeed(float t)
{
	m_BodyDownMotionSpeed = t;

	for (
		auto it = m_DownBodyBoneNumArray.begin();
		it != m_DownBodyBoneNumArray.end();
	++it
		)
	{
		m_pAnimeMesh->SetSpeed(*it, 0, t);
	}
}

//���[�V�����X�V
void CharacterRenderer::Update(float t)
{
	m_pAnimeMesh->Animation(t);
	m_pAnimeMesh->Update();

    m_pAnimeMesh->TransMatrix = m_TransMatrix;
}

//���[���h��ԏ�ł̃{�[���s��𓾂�
void CharacterRenderer::GetWorldBoneMatirx(Matrix& Out, int BoneNum)
{
    GetLocalBoneMatrix(Out, BoneNum);

    Out *= m_TransMatrix;
}

//���[�J�����(�L�����N�^���)��ł̃{�[���s��𓾂�
void CharacterRenderer::GetLocalBoneMatrix(Matrix& Out, int BoneNum)
{
    Out = *m_pAnimeMesh->GetBone(BoneNum);
}

//�`��
void CharacterRenderer::GbufRender(
    iexShader*        pShader,                       //�V�F�[�_�[�N���X
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //�e�N�j�b�N�Ǘ��N���X
    )
{
    char str[256];
    pSetter->NoTexture(str, 256);

    m_pAnimeMesh->Render(pShader, str);
}

void CharacterRenderer::MasterRender()
{
    typedef std::map<int, char*> Techniques;
    Techniques tecs;

    char FaceTechnique[] = { "DeffLightNoSp" };
    char HairTechnique[] = { "DeffLightNoSp" };
    char BodyTechnique[] = { "DeffLightNoSp" };


  //  m_HDR = Vector3(0.08f, 0.05f, 0.00f);

    tecs.insert(Techniques::value_type(0, FaceTechnique));
    tecs.insert(Techniques::value_type(1, FaceTechnique));
    tecs.insert(Techniques::value_type(2, FaceTechnique));
    tecs.insert(Techniques::value_type(3, FaceTechnique));
    tecs.insert(Techniques::value_type(4, FaceTechnique));
    tecs.insert(Techniques::value_type(5, FaceTechnique));
    tecs.insert(Techniques::value_type(6, HairTechnique));
    tecs.insert(Techniques::value_type(7, HairTechnique));
    tecs.insert(Techniques::value_type(8, HairTechnique));
    tecs.insert(Techniques::value_type(9, HairTechnique));

    shader->SetValue("g_HDR_Color", m_HDR);

    m_pAnimeMesh->Render(shader, tecs);
}

void CharacterRenderer::DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type)
{
    char str[256];
    strcpy_s<256>(str, pTec);

    m_pAnimeMesh->Render(pShader, str);
}

void CharacterRenderer::Initialize()
{
	m_BodyUpMotionSpeed = m_BodyDownMotionSpeed = 1;
	m_BodyUpMotion = m_BodyDownMotion = 0;
	D3DXMatrixIdentity(&m_TransMatrix);
}