#include "CharacterBase.h"
#include "CharacterManager.h"
#include "CharacterFunction.h"
#include "../UI/PlayerCursorBillbord.h"
#include "../Ball/UsualBall.h"

//***************************************************
//		�L�����N�^�N���X
//***************************************************

const CharacterBase::CommonParams CharacterBase::m_CommonParams =
{
    0.0f,          //�n�ʂ̍���
    -0.02f,        //�d�͉����x
};

CharacterBase::CharacterBase(const PlayerInfo& info) :
m_PlayerInfo(info),
m_PhysicObj(this),
m_StateType(State::Freeze)
{

	m_Params.win = 0;
    m_Params.camera_draw = true;
	m_Params.pos = Vector3Zero;
	m_Params.move = Vector3Zero;
	m_Params.hitScale = 1.5f;
	m_Params.angle = 0;
	m_Params.HP = 100;
    m_Params.maxHP = 100;
	m_Params.SP = 0;
    m_Params.size = 1.2f;

    //�J�[�\���t�h
    new PlayerCursorBillbord(this);

	//�}�l�[�W���ɓo�^
	DefCharacterMgr.Add(this);
  
}

CharacterBase::~CharacterBase()
{
	//�}�l�[�W������폜
	DefCharacterMgr.Erace(this);
}

COLOR CharacterBase::GetPlayerColor(PlayerNum::Value number)
{
    MyAssert(((int)number >= 0 && (int)number < 4) , "�F�����݂��Ȃ��v���C���[�ԍ��������ɑ����܂��� num= %d ", (int)number);

    static const COLOR player_colors[]=
    {
        0xFFFF2000,
        0xFF00FFFF,
        0xFFFFFF00,
        0xFF00FF00
    };

    return player_colors[number];
}

COLORf CharacterBase::GetPlayerColorF(PlayerNum::Value number)
{
    
    COLORf ret(1, 1, 1, 1);

    switch ((int)number)
    {
    case 0: ret = COLORf(1,1,0.145f,0); break;
    case 1: ret = COLORf(1,0,1,1); break;
    case 2: ret = COLORf(1,1,1,0); break;
    case 3: ret = COLORf(1,0,1,0); break;

    default:
        MyAssert(false, "�F�����݂��Ȃ��v���C���[�ԍ��������ɑ����܂��� num= %d ", (int)number);
        break;
    }

    return ret;
}

//�����蔻����Ƃ���W�𓾂�
Vector3 CharacterBase::CalcHitCheckPos()const
{
    Vector3 ret = m_Params.pos;

    ret.y += UsualBall::UsualBallShotY;

    return ret;
}

//���E���h���Z�b�g���̃p�����^�[�ɃZ�b�g����
void CharacterBase::ResetRound()
{
    m_Params.HP = m_Params.maxHP;
    m_Params.pos = DefCharacterMgr.GetRoundStartPos(m_PlayerInfo.number);
    m_Params.move = Vector3Zero;
    m_Params.camera_draw = true;
    chr_func::AngleControll(this, Vector3(0, m_Params.pos.y, 0));
}

//���t���[���̍X�V
void CharacterBase::BaseUpdate()
{
    m_PhysicObj.Update();
}

bool CharacterBase::Msg(MsgType mt)
{
    switch (mt)
    {
    case MsgType::_PlayStart:
        m_StateType = State::Usual;
        break;

    case MsgType::_RoundReset:
        m_StateType = State::Freeze;
        ResetRound();
        break;

    case MsgType::_GameSet:
        m_StateType = State::Freeze;
        break;

    case MsgType::_WinPose:
        m_StateType = State::WinPose;
        break;

    default:
        return false;
    }

    return CharacterMsg(mt);
}


CharacterBase::State CharacterBase::GetStateType()const
{
    return m_StateType;
}