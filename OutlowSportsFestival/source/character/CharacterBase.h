#ifndef __CHARACTER_BASE_H__
#define __CHARACTER_BASE_H__

#include "../GameSystem/GameObject.h"
#include "../GameSystem/GameSystem.h"
#include "iextreme.h"

//***************************************************
//		�L�����N�^�N���X
//***************************************************


class CharacterBase :public GameObjectBase
{
private:
	CharacterBase(const CharacterBase&);
public:

	struct PlayerInfo
	{
		PlayerNum::Value		number;			//�v���C���[�ԍ�
		PlayerType::Value		player_type;	//�v���C���[�^�C�v
		StrongType::Value		strong_type;	//�����^�C�v
		CharacterType::Value	chr_type;		//�L�����N�^�^�C�v
	};

	struct Params
	{
		UINT                win;        //���������E���h��
		Vector3				pos;		//���W
		Vector3				move;		//�ړ���
		float				hitScale;	//�����蔻��̉~�̑傫��
		float				angle;		//�A���O��(���W�A��)
		float				maxHP;		//�ő�̗͂O�`
		float				HP;			//�O�`
		float				SP;			//�O�`�P�O�O
	};

    //�L�����N�^���ʃp�����[�^
    struct CommonParams
    {
        float  GroundY; //�n�ʂ̍���    
        float  Glavity; //�d�͉����x
    };

    static const CommonParams m_CommonParams;

	const PlayerInfo	m_PlayerInfo; //�v���C���[���ɂ���\����
	Params				m_Params;     //�L�����N�^��{�p�����[�^�[

   
	CharacterBase(const PlayerInfo& info);
	virtual ~CharacterBase();

    static COLOR GetPlayerColor(PlayerNum::Value number);

	virtual bool Update() = 0;	//���̊֐���false��Ԃ���delete�����
	virtual bool Msg(MsgType mt) = 0;	//��M�ł������b�Z�[�W��true��
};

#endif