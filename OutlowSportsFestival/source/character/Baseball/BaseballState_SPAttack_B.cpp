#include "BaseballState_SPAttack_B.h"
#include "BaseballPlayerState.h"
#include "Computer\/BaseballPlayerState_ComMove.h"
#include "../../GameSystem/GameController.h"
#include "../CharacterFunction.h"
#include "../CharacterManager.h"

#include "../../Effect/GlavityLocus.h"
#include "../../Effect/BlurImpact.h"
#include "../../Effect/ThunderEffect.h"
#include "../../Camera/Camera.h"
#include "../../Sound/Sound.h"
#include "../../Effect/SpecialAttackEffect.h"
#include "../../GameSystem/ResourceManager.h"


//�U����
const float BaseballState_SPAttack_B::m_sDamageValue = 45.0f;


BaseballState_SPAttack_B::BaseballState_SPAttack_B(BaseballPlayer* b) :
m_pChr(b),
m_pStateFunc(&BaseballState_SPAttack_B::State_PreAtk),
m_StateTimer(0),
m_BatScale(0.1f, 0.1f, 0.1f)
{
    {
        //�_���[�W������
        m_Damage.m_Enable = false;
        m_Damage.AddSkillGaugeValue = 0.0f;
        m_Damage.m_Param.width = 1.5f;
        m_Damage.m_Vec = chr_func::GetFront(m_pChr);
        m_Damage.m_VecPower.x = 1.2f;
        m_Damage.m_VecPower.y = 0.5f;
        m_Damage.m_VecType = DamageCapsure::DamageVecType::MemberParam;
        m_Damage.pParent = m_pChr;
        m_Damage.type = DamageBase::Type::_VanishDamage;
        m_Damage.Value = m_sDamageValue;
    }


    //�o�b�g���b�V�����쐬
    m_pBatMesh = new MeshRenderer(
        DefResource.Get(Resource::MeshType::Bat),
        false,
        MeshRenderer::RenderType::UseColor
        );

    //�����X�V
    UpdateBatMesh();
}


BaseballState_SPAttack_B::~BaseballState_SPAttack_B()
{
    delete m_pBatMesh;
}

// �X�e�[�g�J�n
void  BaseballState_SPAttack_B::Enter(BaseballPlayer* b)
{
    //���Ԃ��Ƃ߂�
    std::list<LpGameObjectBase> UpdateObjList;

    UpdateObjList.push_back(b);

    DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 50, true);

    //�G�t�F�N�g
    new SpecialAttackEffect(b, 50);

    //SE
    Sound::Play(Sound::Skill);

    //���[�V�������Z�b�g
    m_pChr->SetMotion(baseball_player::_mb_SpAtk_B);

    //�ړ��l������
    chr_func::ResetMove(b);
}

// �X�e�[�g���s
void BaseballState_SPAttack_B::Execute(BaseballPlayer* b)
{
    //�J�E���g�i�s
    m_StateTimer++;

    //���f���X�V
    b->ModelUpdate(2);

    //�X�e�[�g���s
    (this->*m_pStateFunc)();

    //��{�X�V
    chr_func::UpdateAll_DontCheckDamage(b);
    chr_func::CreateTransMatrix(b, &b->getNowModeModel()->m_TransMatrix);
}

// �X�e�[�g�I��
void BaseballState_SPAttack_B::Exit(BaseballPlayer* b)
{
    //�X�L���Q�[�W���Ȃ���
  //  chr_func::ResetSkillGauge(m_pChr);
}

//-------------------------------------------------------------------//
//                            private
//-------------------------------------------------------------------//

void BaseballState_SPAttack_B::SetState(StateFunc state)
{
    m_StateTimer = 0;
    m_pStateFunc = state;
}

void BaseballState_SPAttack_B::UpdateBatMesh()
{
    Matrix T;

    //�o�b�g������{�[���s����擾
    m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    //Z�����t���ۂ��̂Ŕ��](���艟�������ǁc
    {
        T._31 = -T._31;
        T._32 = -T._32;
        T._33 = -T._33;
    }

    //�X�P�[�������𐳋K��
    D3DXMatrixWorldScaleNormalize(T);

    //�o�b�g�̃X�P�[���ɃZ�b�g
    {
        T._11 *= m_BatScale.x;
        T._12 *= m_BatScale.x;
        T._13 *= m_BatScale.x;

        T._21 *= m_BatScale.y;
        T._22 *= m_BatScale.y;
        T._23 *= m_BatScale.y;

        T._31 *= m_BatScale.z;
        T._32 *= m_BatScale.z;
        T._33 *= m_BatScale.z;
    }

    //�Z�b�g
    m_pBatMesh->SetMatrix(T);
}

void BaseballState_SPAttack_B::UpdateDamagePos()
{
    Vector3 Vec;
    Matrix T;

    //�o�b�g�̃{�[���s����擾
    m_pChr->getNowModeModel()->GetWorldBoneMatirx(T, 32);

    //�o�b�g�̐L�т�x�N�g��
    Vec = -Vector3(T._31, T._32, T._33);
    Vec.Normalize();

    //�_���[�W�ʒu���Z�b�g
    m_Damage.m_Param.pos1 = Vector3(T._41, T._42, T._43);
    m_Damage.m_Param.pos2 = m_Damage.m_Param.pos1 + Vec * 9.0f;

}


void BaseballState_SPAttack_B::OnHit()
{
    //SE
    Sound::Play(Sound::BaseBall_SP);

    //���Ԃ��Ƃ߂�
    {
        std::list<LpGameObjectBase> UpdateObjList;

        DefGameObjMgr.FreezeOtherObjectUpdate(UpdateObjList, 20, true);
    }

    //�G�t�F�N�g

    
}



void BaseballState_SPAttack_B::State_PreAtk()
{
    const int AllFrame = 50;
    const int BatScaleUpFrame = 35;

    const Vector3 batScaleTarget(0.2f, 0.2f, 0.2f);

    //�r�Ƀo�b�g���Z�b�g
    UpdateBatMesh();

    //�o�b�g��傫������
    if (m_StateTimer > BatScaleUpFrame)
    {
        m_BatScale += (batScaleTarget - m_BatScale)*0.15f;
    }

    //�t���[���o�߂ōU���X�e�[�g��
    if (m_StateTimer > AllFrame)
    {
        SetState(&BaseballState_SPAttack_B::State_Atk);
    }
}

void BaseballState_SPAttack_B::State_Atk()
{
    const int AllFrame = 20;
    const int BatSmallFrame = 5;
    const int DamageEndFrame = 5;


    //�_���[�W�ʒu���Z�b�g
    UpdateDamagePos();

    //�r�Ƀo�b�g���Z�b�g
    UpdateBatMesh();

    //�_���[�W�̗L���E�������Z�b�g
    m_Damage.m_Enable = (m_StateTimer < DamageEndFrame);


    //�o�b�g������������
    if (m_StateTimer > BatSmallFrame)
    {
        m_BatScale *= 0.85f;
    }

    //�t���[���o�߂Œʏ�X�e�[�g��
    if (m_StateTimer > AllFrame)
    {
        m_pChr->SetState(BaseballState_PlayerControll_Move::GetPlayerControllMove(m_pChr));
    }
}

