#include "TennisSpecialAtk.h"
#include "../../Ball/UsualBall.h"
#include "../../Camera/Camera.h"
#include "../../Collision/Collision.h"


TennisSpecialBall::TennisSpecialBall(TennisPlayer* t, BallBase::Params& param) :
m_Locus(20),
m_DeleteFlag(false),
m_pTennis(t)
{
    LPIEXMESH		pBallMesh;

    //�p�����[�^���
    m_Params = param;

    //�_���[�W����̃p�����[�^����
    m_Damage.pBall = this;
    m_Damage.pParent = param.pParent;
    m_Damage.m_Param.size = 1;	//�傫���̓{�[���ɂ���ĈقȂ�?
    m_Damage.type = DamageBase::Type::_WeekDamage;
    m_Damage.Value = 0.1f;
    m_Damage.m_Enable = true;
    UpdateDamageClass();

    //�{�[���̃��b�V�����쐬
    UsualBall::GetBallMesh(param.pParent->m_PlayerInfo.chr_type, &pBallMesh);

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

TennisSpecialBall::~TennisSpecialBall()
{
    delete m_pMeshRenderer;
}


bool TennisSpecialBall::Update()
{
    UpdateMove();
    UpdateMesh();
    UpdateDamageClass();

    if (m_Params.type != BallBase::Type::_DontWork)
    {
        UpdateWallCheck();
    }

    Vector3 v;
    Vector3Cross(v, m_Params.move, DefCamera.GetForward());
    v.Normalize();

    m_Locus.AddPoint(m_Params.pos, v);

    return !m_DeleteFlag && !isOutofField() && m_DeleteFrame > 0;
}


bool TennisSpecialBall::Msg(MsgType mt)
{

    return false;
}


bool TennisSpecialBall::isOutofField()const
{
    return Vector3Length(m_Params.pos) > 100;
}


void TennisSpecialBall::UpdateDamageClass()
{
    m_Damage.vec = m_Params.move;
    m_Damage.m_Param.pos = m_Params.pos;
}

void TennisSpecialBall::UpdateMesh()
{
    //���b�V���̃��[���h�ϊ��s����X�V����

    Matrix m;
    const float s = UsualBall::GetBallScale(m_Params.pParent->m_PlayerInfo.chr_type);

    D3DXMatrixScaling(&m, s, s, s);	//�傫���̓{�[���ɂ���ĕς���K�v������

    m._41 = m_Params.pos.x;
    m._42 = m_Params.pos.y;
    m._43 = m_Params.pos.z;

    m_pMeshRenderer->SetMatrix(m);
}

void TennisSpecialBall::UpdateLocusColor()
{
    const DWORD Color = CharacterBase::GetPlayerColor(m_Params.pParent->m_PlayerInfo.number);

    m_Locus.m_StartParam.Color = Vector4(
        float((Color >> 16) & 0xFF) / 255.f,
        float((Color >> 8) & 0xFF) / 255.f,
        float(Color & 0xFF) / 255.f,
        0.5f
        );

    m_Locus.m_EndParam.Color = Vector4(1, 1, 1, 0);
}

void TennisSpecialBall::UpdateMove()
{

    m_Params.pos += m_Params.move;

    //�G�ɓ������Ă�����U��������Ȃ���
    if (m_Damage.HitCount > 0)
    {
        //�U������̂Ȃ���Ԃɂ���
        ToNoWork();
    }
}

void TennisSpecialBall::UpdateWallCheck()
{
    Vector3 Out, Pos(m_Params.pos), Vec(m_Params.move);
    float Dist = m_Params.move.Length()*2.0f;
    int Material;

    Vec.Normalize();

    if (DefCollisionMgr.RayPick(
        &Out,
        &Pos,
        &Vec,
        &Dist,
        &Material,
        CollisionManager::RayType::_Usual
        )
        )
    {
        //�U������̂Ȃ���Ԃɂ���
        ToNoWork();
    }

}

void TennisSpecialBall::Counter(CharacterBase* pCounterCharacter)
{
    m_Damage.pParent = m_Params.pParent = pCounterCharacter;

    UpdateLocusColor();
}

void TennisSpecialBall::ToNoWork()
{
    //�U������̂Ȃ���Ԃɂ���

    if (m_DeleteFlag)
    {
        return;
    }

    m_Params.type = BallBase::Type::_DontWork;
    m_Damage.m_Enable = false;
    m_DeleteFlag = true;

    m_BaseMatrix = m_pMeshRenderer->GetMatrix();

    m_BaseMatrix._41 = 0;
    m_BaseMatrix._42 = 0;
    m_BaseMatrix._43 = 0;
}