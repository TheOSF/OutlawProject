#include "CharacterRenderer.h"
#include "../IexSystem/System.h"

//*************************************************
//	キャラクタ用メッシュクラス
//*************************************************


//通常コンストラクタ
CharacterRenderer::CharacterRenderer(BlendAnimationMesh* pAnimeMesh) :
m_pAnimeMesh(pAnimeMesh),
m_UsePartsMotion(false),
m_HDR(0,0,0)
{
	Initialize();
}

//部位モーションをする際に
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

//マテリアルがどの部位かを指定する(描画切り替えのため)
void CharacterRenderer::SetMaterialRenderType(int MaterialNum, RenderType Type)
{
    static const char* Techniques[] =
    {
        "CharacterSkin",
        "DeffLightNoSp",
        "DeffLightNoSp",
        "CharacterSkin",
        "DeffLightNoSp",
    };

    MyAssert(MaterialNum >= 0 && MaterialNum < m_pAnimeMesh->GetNumMaterial(), "描画方法の指定で存在しないマテリアル番号が指定されました Material=%d", MaterialNum);

    if (m_Techniques.count(MaterialNum) > 0)
    {
        m_Techniques[MaterialNum] = Techniques[(int)Type];
    }
    else
    {
        m_Techniques.insert(Techniques::value_type(MaterialNum, Techniques[(int)Type]));
    }
}

//通常のモーションセット
void CharacterRenderer::SetMotion(int m)
{
	const int nb = m_pAnimeMesh->GetNumBone();

	for (int i = 0; i < nb; ++i)
	{
		m_pAnimeMesh->SetMotion(m, i, 0);
	}
}


//上半身・下半身ごとのボーションセット
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


//通常のモーションスピードセット
void CharacterRenderer::SetMotionSpeed(float t)
{
	m_BodyUpMotionSpeed = m_BodyDownMotionSpeed = t;

	const int nb = m_pAnimeMesh->GetNumBone();

	for (int i = 0; i < nb; ++i)
	{
		m_pAnimeMesh->SetSpeed(i, 0, t);
	}
}

//上半身・下半身ごとのスピードセット
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

//モーション更新
void CharacterRenderer::Update(float t)
{
	m_pAnimeMesh->Animation(t);
	m_pAnimeMesh->Update();

    m_pAnimeMesh->TransMatrix = m_TransMatrix;
}

//ワールド空間上でのボーン行列を得る
void CharacterRenderer::GetWorldBoneMatirx(Matrix& Out, int BoneNum)
{
    GetLocalBoneMatrix(Out, BoneNum);

    Out *= m_TransMatrix;
}

//ローカル空間(キャラクタ空間)上でのボーン行列を得る
void CharacterRenderer::GetLocalBoneMatrix(Matrix& Out, int BoneNum)
{
    Out = *m_pAnimeMesh->GetBone(BoneNum);
}

//描画
void CharacterRenderer::GbufRender(
    iexShader*        pShader,                       //シェーダークラス
    DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
    )
{
    char str[256];
    pSetter->NoTexture(str, 256);

    m_pAnimeMesh->Render(pShader, str);
}

void CharacterRenderer::MasterRender()
{
    shader->SetValue("g_HDR_Color", m_HDR);

    m_pAnimeMesh->Render(shader, m_Techniques);
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

    //デフォルトの描画テクニックを設定
    for (int i = m_pAnimeMesh->GetNumMaterial() - 1; i >= 0; --i)
    {
        SetMaterialRenderType(i, RenderType::Normal);
    }
}