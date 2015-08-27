#include "UsualBall.h"
#include "../character/CharacterBase.h"
#include "../Render/MeshRenderer.h"
#include "../Camera/Camera.h"
#include "../GameSystem/ResourceManager.h"

UsualBall::UsualBall(
    BallBase::Params	params,			//�{�[���p�����[�^
    DamageBase::Type	damage_type,	//�_���[�W����̃^�C�v
    float				damage_val		//�_���[�W��
    ) :
    m_FreezeCount(0),
    m_FreezeDeleteFrame(60),
    m_Locus(20)
{
	LPIEXMESH		pBallMesh;

	//�p�����[�^���
	m_BallBase.m_Params = params;

	//�_���[�W����̃p�����[�^����
	m_Damage.pBall = &m_BallBase;
	m_Damage.pParent = params.pParent;
	m_Damage.m_Param.size = 1;	//�傫���̓{�[���ɂ���ĈقȂ�?
	m_Damage.type = damage_type;
	m_Damage.Value = damage_val;
	UpdateDamageClass();

	//�{�[���̃��b�V�����쐬
	GetBallMesh(params.pParent->m_PlayerInfo.chr_type, &pBallMesh);

	//���b�V���̃����_���[�쐬(�ŏI�I�Ƀ��b�V�����g���܂킵�ĕ`�悷��ׂ�)
	m_pMeshRenderer = new MeshRenderer(
		pBallMesh,
		false,
        MeshRenderer::RenderType::UseColor
		);

	D3DXQuaternionIdentity(&m_Ballrot);

    UpdateMesh();

    //�O�Ղ̐ݒ�
    m_Locus.m_Division = 0;
    m_Locus.m_StartParam.Width = 0.4f;
    m_Locus.m_EndParam.Width = 0.1f;

    UpdateLocusColor();
}

UsualBall::~UsualBall()
{
	delete	m_pMeshRenderer;
}

bool UsualBall::GetBallMesh(
	CharacterType::Value	type,
	LPIEXMESH*				ppOut)
{
	*ppOut = nullptr;

	switch (type)
	{
	case CharacterType::_Tennis:
        *ppOut = DefResource.Get(Resource::MeshType::Tennis_ball);
		break;
	case CharacterType::_Baseball:
        *ppOut = DefResource.Get(Resource::MeshType::BaseBall_ball);
		break;
	case CharacterType::_Americanfootball:
        *ppOut = DefResource.Get(Resource::MeshType::Amefoot_ball);
		break;
	case CharacterType::_Soccer:
        *ppOut = DefResource.Get(Resource::MeshType::Soccer_ball);
		break;
	case CharacterType::_Lacrosse:
        *ppOut = DefResource.Get(Resource::MeshType::Lacrosse_ball);
		break;
	case CharacterType::_Volleyball:
        *ppOut = DefResource.Get(Resource::MeshType::Volley_ball);
	default:
		break;
	}

	return (*ppOut != nullptr);
}


float UsualBall::GetBallScale(
    CharacterType::Value	type    //�{�[���̃L�����N�^�^�C�v
    )
{
    switch (type)
    {
    case CharacterType::_Americanfootball:
        return 0.0045f;

    case CharacterType::_Baseball:
        return 0.0045f;

    case CharacterType::_Lacrosse:
        return 0.0045f;

    case CharacterType::_Soccer:
        return 0.0045f;


    case CharacterType::_Tennis:
        return 0.0045f;


    case CharacterType::_Volleyball:
        return 0.0045f;

    default:
        break;
    }

    MyAssert(false, "���݂��Ȃ��^�C�v�̃L�����N�^�^�C�v��UsualBall::GetBallScale�ɓn����܂����@type= %d ", (int)type);


    return 0;
}

bool UsualBall::Update()
{
	m_BallBase.m_Params.pos += m_BallBase.m_Params.move;

	if (isOutofField())
	{
		m_FreezeCount = m_FreezeDeleteFrame;
	}

	UpdateMesh();
	UpdateDamageClass();

    Vector3 v;
    Vector3Cross(v, m_BallBase.m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_BallBase.m_Params.pos, v);

	return m_FreezeCount < m_FreezeDeleteFrame;
}


bool UsualBall::Msg(MsgType mt)
{

	return false;
}


bool UsualBall::isOutofField()const
{
	return Vector3Length(m_BallBase.m_Params.pos) > 100;
}


void UsualBall::UpdateDamageClass()
{
	m_Damage.vec = m_BallBase.m_Params.move;
	m_Damage.m_Param.pos = m_BallBase.m_Params.pos;
}

void UsualBall::UpdateMesh()
{
	//���b�V���̃��[���h�ϊ��s����X�V����

	Matrix m;
    const float s = GetBallScale(m_BallBase.m_Params.pParent->m_PlayerInfo.chr_type);

    D3DXMatrixScaling(&m, s, s, s);	//�傫���̓{�[���ɂ���ĕς���K�v������
    
	m._41 = m_BallBase.m_Params.pos.x;
	m._42 = m_BallBase.m_Params.pos.y;
	m._43 = m_BallBase.m_Params.pos.z;

	m_pMeshRenderer->SetMatrix(m);
}

void UsualBall::UpdateLocusColor()
{
    const DWORD Color = CharacterBase::GetPlayerColor(m_BallBase.m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Vector4(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f,
        0.5f
        );

    m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void UsualBall::SetHDR()
{
    const DWORD Color = CharacterBase::GetPlayerColor(m_BallBase.m_Params.pParent->m_PlayerInfo.number);

    m_pMeshRenderer->m_HDR = Vector3(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f
        );
}