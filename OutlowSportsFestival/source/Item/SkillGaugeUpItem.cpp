#include "SkillGaugeUpItem.h"
#include "../GameSystem/ResourceManager.h"
#include "../character/CharacterFunction.h"
#include "../character/CharacterManager.h"

#include "../Effect/ImpactLightObject.h"
#include "../Effect/BlurImpact.h"
#include "../Sound/Sound.h"
#include "../Collision/Collision.h"
#include "../GameSystem/GameController.h"

SkillGaugeUpItem::SkillGaugeUpItem(CrVector3 pos, CrVector3 move) :
m_LiveFlag(true),
m_pMeshRenderer(nullptr),
m_Pos(pos),
m_Move(move)
{
    //���b�V�������_���[�쐬
    m_pMeshRenderer = new MeshRenderer(
        DefResource.Get(Resource::MeshType::Soccer_ball),
        false,
        MeshRenderer::RenderType::UseColorSpecular
        );

    m_pMeshRenderer->m_HDR = Vector3(1, 0.5f, 0);

    MeshUpdate();
}


SkillGaugeUpItem::~SkillGaugeUpItem()
{
    delete m_pMeshRenderer;
}

void SkillGaugeUpItem::MeshUpdate()
{
    const float s = 0.1f;
    Matrix m;

    D3DXMatrixScaling(&m, s, s, s);

    m._41 = m_Pos.x;
    m._42 = m_Pos.y;
    m._43 = m_Pos.z;

    m_pMeshRenderer->SetMatrix(m);
}

bool SkillGaugeUpItem::CalcNearCharacter(CharacterBase** ppOut)const
{
    //�������Ă���L�����N�^�������true�Ԃ�
    const float HitLen = 3.0f;
    CharacterManager::CharacterMap ChrMap = DefCharacterMgr.GetCharacterMap();

    float MostNear = 3.0f, len;

    //�߂�l��������
    *ppOut = nullptr;


    for (auto it : ChrMap)
    {
        //����ł���L���������O
        if (chr_func::isDie(it.first))
        {
            continue;
        }

        len = Vector3Distance(m_Pos, it.first->m_Params.pos);

        if (MostNear > len)
        {
            MostNear = len;
            *ppOut = it.first;
        }
    }

    return (*ppOut) != nullptr;
}

bool SkillGaugeUpItem::Update()
{
    CharacterBase* HitChr = nullptr;
    const float MinY = 2.0f;

    //�ł��߂��L�����N�^���Z�o
    if (CalcNearCharacter(&HitChr))
    {
        //�擾���Ă����ꍇ

        //���g��������
        m_LiveFlag = false;

        //�G�t�F�N�g���o����
        new ImpactLightObject(
            m_Pos,
            25.0f,
            Vector3(1, 0.5f, 0),
            0.1f
            );

        new BlurImpactSphere(
            m_Pos,
            15.0f,
            80.0f,
            20
            );

        //�r�d��炵��
        Sound::Play(Sound::Change);

        //�Q�[�W���㏸
        chr_func::AddSkillGauge(HitChr, 0.5f);

        //�R���g���[����U��
        chr_func::SetControllerShock(
            HitChr,
            1.0f,
            0.15f
            );
    }
    else
    {
        //�����Ƃ��̗͂̒Ⴂ�v���C���[�Ɍ������Ĉړ����� <- �w

        //�d�͂ɂ��������Ĉړ�
        Vector3  Out, Pos(m_Pos), Vec(m_Move.x, 0, m_Move.z);
        float Dist = Vector3XZLength(m_Move);
        int Mat;

        Vec.Normalize();

        if (Vec != Vector3Zero&&
            DefCollisionMgr.RayPick(
            &Out,
            &Pos,
            &Vec,
            &Dist,
            &Mat,
            CollisionBase::RayType::_Character
            ) != nullptr)
        {
            m_Pos = Out;
            
            Out = Vector3(m_Move.x, 0, m_Move.z);
            Out = Vector3Refrect(Out, Vec);

            m_Move.x = Out.x;
            m_Move.z = Out.z;
        }
        else
        {
            m_Pos.x += m_Move.x;
            m_Pos.z += m_Move.z;
        }

        m_Move.x *= 0.95f;
        m_Move.z *= 0.95f;

        m_Move.y += -0.01f;

        if (m_Pos.y <= MinY)
        {
            m_Pos.y = MinY;
            m_Move.y = -m_Move.y*0.3f;
        }

        m_Pos.y += m_Move.y;
    }

    MeshUpdate();

    return m_LiveFlag;
}

bool SkillGaugeUpItem::Msg(MsgType mt)
{
    return false;
}