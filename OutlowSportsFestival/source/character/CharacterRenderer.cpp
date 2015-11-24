#include "CharacterRenderer.h"
#include "../IexSystem/System.h"
#include "../Camera/Camera.h"

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

//�}�e���A�����ǂ̕��ʂ����w�肷��(�`��؂�ւ��̂���)
void CharacterRenderer::SetMaterialRenderType(int MaterialNum, RenderType Type)
{
    static const char* Techniques[] =
    {
        "CharacterSkin",
        "CharacterHair",
        "CharacterClothes",
        "CharacterSkin",
        "DeffLightNoSp",
    };

    MyAssert(MaterialNum >= 0 && MaterialNum < m_pAnimeMesh->GetNumMaterial(), "�`����@�̎w��ő��݂��Ȃ��}�e���A���ԍ����w�肳��܂��� Material=%d", MaterialNum);

    if (m_Techniques.count(MaterialNum) > 0)
    {
        m_Techniques[MaterialNum] = Techniques[(int)Type];
    }
    else
    {
        m_Techniques.insert(Techniques::value_type(MaterialNum, Techniques[(int)Type]));
    }
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

//���[���h��ԏ�ł̃{�[�����W�𓾂�
Vector3 CharacterRenderer::GetWorldBonePos(int BoneNum)
{
    return Vector3MulMatrix(GetLocalBonePos(BoneNum), m_TransMatrix);
}

//���[�J�����(�L�����N�^���)��ł̃{�[�����W�𓾂�
Vector3 CharacterRenderer::GetLocalBonePos(int BoneNum)
{
    Matrix* m = m_pAnimeMesh->GetBone(BoneNum);
    return Vector3(m->_41, m->_42, m->_43);
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
    shader->SetValue("g_HDR_Color", m_HDR);
    shader->SetValue("g_Color", D3DXVECTOR4(m_ClothesColor.x, m_ClothesColor.y, m_ClothesColor.z, 1.0f));
    shader->SetValue("g_Lighting_Color", Vector3(0, 0, 0));

    m_pAnimeMesh->Render(shader, m_Techniques);
}

void CharacterRenderer::DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type)
{
    char str[256];
    strcpy_s<256>(str, pTec);

    m_pAnimeMesh->Render(pShader, str);
}

void CharacterRenderer::CalcZ()
{  
    m_SortZ = DefCamera.GetCameraZ(Vector3(m_TransMatrix._41, m_TransMatrix._42, m_TransMatrix._43));
}

void CharacterRenderer::Render()
{
    if (m_OutlineVisible)
    {
        shader->SetValue("g_Outline_Color", m_OutlineColor.toVector4());

        m_pAnimeMesh->Render(shader, "OutLine");
    }
}

void CharacterRenderer::Initialize()
{
	m_BodyUpMotionSpeed = m_BodyDownMotionSpeed = 1;
	m_BodyUpMotion = m_BodyDownMotion = 0;
	D3DXMatrixIdentity(&m_TransMatrix);

    m_ClothesColor = Vector3Zero;

    //m_OutlineColor.SetColor(0, 2.0f, 1.33f, 1.0f);
    //m_OutlineColor.SetColor(0, 1.8f, 1.25f, 1.0f);
    m_OutlineColor.SetColor(0, 0, 0, 0);
    m_OutlineVisible = false;

    m_HDR = Vector3Zero;

    //�f�t�H���g�̕`��e�N�j�b�N��ݒ�
    for (int i = m_pAnimeMesh->GetNumMaterial() - 1; i >= 0; --i)
    {
        SetMaterialRenderType(i, RenderType::Normal);
    }
}