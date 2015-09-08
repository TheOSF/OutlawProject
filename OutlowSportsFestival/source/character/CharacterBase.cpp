#include "CharacterBase.h"
#include "CharacterManager.h"
#include "CharacterFunction.h"


//***************************************************
//		�L�����N�^�N���X
//***************************************************

const CharacterBase::CommonParams CharacterBase::m_CommonParams =
{
    0.0f,          //�n�ʂ̍���    
    -0.01f,        //�d�͉����x
};

CharacterBase::CharacterBase(const PlayerInfo& info):
m_PlayerInfo(info)
{

	m_Params.win = 0;
	m_Params.pos = Vector3Zero;
	m_Params.move = Vector3Zero;
	m_Params.hitScale = 1.5f;
	m_Params.angle = 0;
	m_Params.HP = 100;
    m_Params.maxHP = 100;
	m_Params.SP = 0;
    m_Params.size = 1.2f;

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
        0xFFFF0000,
        0xFF00FFFF,
        0xFFFFFF00,
        0xFF00FF00
    };

    return player_colors[number];
}

//���E���h���Z�b�g���̃p�����^�[�ɃZ�b�g����
void CharacterBase::ResetRound()
{
    m_Params.HP = m_Params.maxHP;
    m_Params.pos = DefCharacterMgr.GetRoundStartPos(m_PlayerInfo.number);
    m_Params.move = Vector3Zero;
    chr_func::AngleControll(this, Vector3(0, m_Params.pos.y, 0));
}