#include "CharacterComputerMove.h"
#include "../CharacterManager.h"
#include "../CharacterFunction.h"
#include "../../Collision/Collision.h"
#include "../../debug/DebugDraw.h"

////////////////////////////�x�X�g�����������_������ĂȂ�

CharacterComputerMove::InParam::InParam(const InParam& p)
{
    m_BestLenList = p.m_BestLenList;
}

//------------------------------------------------------------//
//            �R���s���[�^�p�̈ړ��N���X
//------------------------------------------------------------//


CharacterComputerMove::CharacterComputerMove(CharacterBase* p, const InParam& param) :
m_pChr(p),
m_InParam(param),
m_MoveVec(0, 0),
m_StateTimer(0),
m_pTargetChr(nullptr),
m_MoveTargetPos(Vector3Zero),
m_StopFrame(0),
m_pStateFunc(&CharacterComputerMove::State_TargetSelect),
m_ViewTargetChr(false),
m_SelectBestLen(0),
m_MovePosRotate(0)
{
    MyAssert(m_InParam.m_BestLenList.size() > 0, "�L�����̃x�X�g�������ݒ肳��Ă��܂���");
}

CharacterComputerMove::~CharacterComputerMove()
{

}


void CharacterComputerMove::Update()
{
    //�t���[���J�E���g
    m_StateTimer = min(m_StateTimer + 1, 10000);

    //�X�e�[�g�֐������s
    (this->*m_pStateFunc)();

#ifdef _DEBUG
    //�f�o�b�O
    new DebugDrawSphere(m_MoveTargetPos + Vector3(0, 2, 0), 0.5f, COLORf(1, 0, 0, 1));
#endif
}

Vector2 CharacterComputerMove::GetMoveVec()
{
    return m_MoveVec;
}

//���̃N���X�����݃^�[�Q�b�g�ɂ��Ă���L�����N�^�𓾂�
bool CharacterComputerMove::GetTargetCharacter(CharacterBase** ppOutTargetChr)const
{
    *ppOutTargetChr = m_pTargetChr;

    return *ppOutTargetChr != nullptr;
}

//�X�e�[�g�Z�b�g
void CharacterComputerMove::SetNewState(StateFunc state)
{
    m_pStateFunc = state;
    m_StateTimer = 0;
}

CharacterComputerMove::StrongParam CharacterComputerMove::GetStrongParam()const
{
    StrongParam ret;

    switch (m_pChr->m_PlayerInfo.strong_type)
    {
    case StrongType::_Weak:
        ret.UpdateTargetChrRatio = 0.01f;
        ret.UpdateMovePosRatio = 0.02f;
        ret.StopRatio = 0.002f;
        ret.RandBestLen = 10.0f;
        break;

    case StrongType::_Usual:
        ret.UpdateTargetChrRatio = 0.02f;
        ret.UpdateMovePosRatio = 0.04f;
        ret.StopRatio = 0.001f;
        ret.RandBestLen = 5.0f;
        break;

    case StrongType::_Strong:
        ret.UpdateTargetChrRatio = 0.1f;
        ret.UpdateMovePosRatio = 0.1f;
        ret.StopRatio = 0.00f;
        ret.RandBestLen = 1.0f;
        break;

    default:
        MyAssert(false, "�Ή��ł��Ȃ���Փx�^�C�v�ł�");
        break;
    }

    return ret;
}

//�^�[�Q�b�g�L�����N�^���X�V����t���[�����ǂ���
bool CharacterComputerMove::isUpdateTargetFrame()const
{
    return frand() <= GetStrongParam().UpdateTargetChrRatio;
}

//�ړ��ڕW�ʒu���X�V����t���[�����ǂ���
bool CharacterComputerMove::isUpdatePositionFrame()const
{
    return frand() <= GetStrongParam().UpdateMovePosRatio && m_StateTimer > 30;
}

//�Ƃ܂�t���[�����ǂ���
bool CharacterComputerMove::isStopStartFrame(int MoveingFrame)const
{
    return frand() <= GetStrongParam().UpdateMovePosRatio;
}

//�ړ��ڕW�̒n�_�Ƌ߂����ǂ���
bool CharacterComputerMove::isNearTargetPos(float OkLen)const
{
    return Vector3Distance(m_pChr->m_Params.pos, m_MoveTargetPos) < OkLen;
}

//�x�X�g�����ɂ���L�����N�^�𓾂�
bool CharacterComputerMove::GetAttackTargetChr(CharacterBase** ppOut)
{
    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
    const float OKlen = 2.5f;

    float MostBestNearLen = OKlen;  //�����Ƃ��߂��L�����N�^�ƃx�X�g�����Ƃ̋�����ۑ�����
    float TempLen = 0.0f;           //���ׂ�L�����N�^�Ɛe�L�����N�^�ւ̋�����ۑ�
    float TempBestLen = 0.0f;       //�v�Z�p��

    //������
    *ppOut = nullptr;

    for (auto &ChrMapIterator : ChrMap)
    {
        CharacterBase* const it = ChrMapIterator.first;

        //���g�Ǝ���ł���L���������O
        if (it == m_pChr || chr_func::isDie(it))
        {
            continue;
        }

        //���ׂ�L�����N�^�Ɛe�L�����N�^�ւ̋�����ۑ�
        TempLen = Vector3Distance(m_pChr->m_Params.pos, it->m_Params.pos);

        //�x�X�g�����Ƃ̒����̍��𑪂�
        for (auto& BestLenIterator : m_InParam.m_BestLenList)
        {
            TempBestLen = BestLenIterator.Len - TempLen;
            TempBestLen = fabsf(TempBestLen);

            //�������߂�������X�V
            if (TempBestLen < MostBestNearLen)
            {
                MostBestNearLen = TempBestLen;
                *ppOut = it;
            }
        }
    }

    return *ppOut != nullptr && 0;
}

bool CharacterComputerMove::GetMostHighHpChr(CharacterBase** ppOut)
{
    const CharacterManager::CharacterMap& ChrMap = DefCharacterMgr.GetCharacterMap();
    RATIO MostHighHpRatio = 0.0f;
    RATIO TempRatio = 0.0f;

    //������
    *ppOut = nullptr;

    for (auto &ChrMapIterator : ChrMap)
    {
        CharacterBase* const it = ChrMapIterator.first;

        //���g�Ǝ���ł���L���������O
        if (it == m_pChr || chr_func::isDie(it))
        {
            continue;
        }

        //�g�o�̎c���Ă闦���擾
        TempRatio = chr_func::GetLifeRatio(it);

        //����������X�V
        if (MostHighHpRatio < TempRatio)
        {
            MostHighHpRatio = TempRatio;
            *ppOut = it;
        }
    }

    return *ppOut != nullptr;
}

//�^�[�Q�b�g�𓾂�
bool CharacterComputerMove::GetTargetChr(CharacterBase** ppOut)
{
    //��{�I��HP�������L�����N�^��_�����A�U���ɓK�����L�����N�^������ꍇ�͂��̃L�����ɂ���

    ////�U���ɓK�����L�����N�^���擾
    //if (GetAttackTargetChr(ppOut))
    //{
    //    //�擾�ł����I�I
    //    return true;
    //}

    //�g�o�̑����L�����N�^���擾
    if (GetMostHighHpChr(ppOut))
    {
        //�擾�ł����I�I
        return true;
    }
    
    *ppOut = nullptr;

    return false;
}

float CharacterComputerMove::GetBestLen()
{
    RATIO value = frand();

    //�x�X�g�����Ƃ̒����̍��𑪂�
    for (auto& BestLenIterator : m_InParam.m_BestLenList)
    {
        if (value < BestLenIterator.Ratio)
        {
            return BestLenIterator.Len;
        }

        value -= BestLenIterator.Ratio;
    }


    MyAssert(false, "�m�������������H");

    return 0.0f;
}

//�ڕW�n�_���݂Ă邩
bool CharacterComputerMove::isViewTargetPos()const
{
    const RADIAN OKangle = D3DXToRadian(20);

    return Vector3Radian(chr_func::GetFront(m_pChr), m_pTargetChr->m_Params.pos - m_pChr->m_Params.pos) < OKangle;
}

//�ړ��ڕW�𓾂�
Vector3 CharacterComputerMove::GetMovePosition()
{
    Vector3 ret = Vector3Zero;

    //��������ړ����ׂ����W���Z�o����
    {
        Vector3 Vec = m_pTargetChr->m_Params.pos;

        Vec.y = 0.0f;
        Vec.Normalize();
        Vec *= -m_SelectBestLen;

        //��]
        {
            Vec =  Vector3RotateAxis(Vector3AxisY, m_MovePosRotate, Vec);
        }

        ret = m_pTargetChr->m_Params.pos + Vec;
    }

    return ret;
}

//�Ƃ܂��Ă���t���[�����擾
int CharacterComputerMove::GetStopFrame()const
{
    const int Min = 20;
    const int Max = 50;
    
    //�����I�����[�I(������ۂ��Ȃ�)
    return Min + (rand() % (Max - Min));
}

//�^�[�Q�b�g�I�l
void CharacterComputerMove::State_TargetSelect()  
{
    //�^�[�Q�b�g���擾
    if (GetTargetChr(&m_pTargetChr))
    {
        //�擾�ł����ꍇ

        //�ێ����鋗�����擾
        m_SelectBestLen = GetBestLen();

        //�ړ��ڕW���v�Z
        m_MoveTargetPos = GetMovePosition();

        //�K�x�ɉ�]������
        m_MovePosRotate = (frand()*2.0f - 1.0f) * D3DXToRadian(90);

        //�����X�e�[�g��
        SetNewState(&CharacterComputerMove::State_Move);
    }
    else
    {
        //�ړ��ڕW�����������I�H
        m_MoveVec = Vector2(0, 0);
    }
}

//�ڕW�Ɍ������Ĉړ�
void CharacterComputerMove::State_Move()
{

    //�������W�X�V�t���[���Ȃ�X�V
    if (isUpdatePositionFrame())
    {
        m_MoveTargetPos = GetMovePosition();
    }

    //�ړ��x�N�g�����쐬
    {
        //�ړ��ڕW�Ƌ߂������ꍇ or ��莞�Ԍo��
        if (isNearTargetPos(4.0f) || m_StateTimer > 150)
        {
            //�X�e�[�g�ڍs
            SetNewState(&CharacterComputerMove::State_NearTargetPos);
        }
        else
        {
            //�ڕW�ɂނ����Ĉړ�����
            m_MoveVec.x = m_MoveTargetPos.x - m_pChr->m_Params.pos.x;
            m_MoveVec.y = m_MoveTargetPos.z - m_pChr->m_Params.pos.z;
        }
    }

    //�ꉞ���K��
    m_MoveVec = Vector2Normalize(m_MoveVec);

}

//�����ȍX�V�͂��邪������ێ�����
void CharacterComputerMove::State_NearTargetPos()
{

    //���łɖڕW�������Ă��Ă��ꍇ�������Ȃ�
    if (m_ViewTargetChr)
    {
        m_MoveVec = Vector2(0, 0);
    }
    else
    {
        //�ڕW�̂ق�������(������ƖڕW�ɂނ��Ĉړ�����
        Vector3 MoveVec = m_pTargetChr->m_Params.pos - m_pChr->m_Params.pos;

        m_MoveVec = Vector2(MoveVec.x, MoveVec.z);

        m_ViewTargetChr = isViewTargetPos();
    }

    //�ꉞ���K��
    m_MoveVec = Vector2Normalize(m_MoveVec);

    //���Ԍo�߂œ����X�e�[�g��
    if (m_StateTimer >= 40)
    {
        m_ViewTargetChr = false;

        SetNewState(&CharacterComputerMove::State_TargetSelect);
    }

}

//��莞�ԗ����~�܂�
void CharacterComputerMove::State_Stop()
{
    //�ړ����Ȃ��I
    m_MoveVec = Vector2(0, 0);

    //�t���[���o�߂œ����X�e�[�g��
    if (m_StateTimer >= m_StopFrame)
    {
        SetNewState(&CharacterComputerMove::State_Move);
    }
}