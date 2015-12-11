#ifndef __BALL_H__
#define __BALL_H__

#include <array>
#include <list>
#include <bitset>
#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"

//*****************************************************
//		�ʃx�[�X�N���X
//*****************************************************

class BallBase
{
public:
	//�ʏ�{�[���ł��̃��[���h���W��ł�y�̒l
	static const float UsualBallShotY;

    //�{�[���ŗLID
    enum BallID :int
    {
        ErrorID = -1 //�G���[�l
    };

	//���̋@�\�̃^�C�v
	enum Type
	{
		_DontWork,				//�`��͂��Ă��邪�A�����蔻��̂Ȃ��{�[��
		_Usual,					//�ʏ�̃{�[��
        _ICantCounter,          //�ł����v���C���[�̂݃J�E���^�[�ł��Ȃ�
		_CantCounter,			//�J�E���^�[�ł��Ȃ��ʏ�̃{�[��
        _Milder,                //�싅�z�[�~���O
		_BaseballSpecialAtk     //�싅�K�E
	};

	//�p�����[�^�\����
	struct Params
	{
		CharacterBase*	pParent;	//���˂����L�����N�^�ւ̃|�C���^
		Type			type;		//�ʂ̃^�C�v
		Vector3			pos;		//�ꏊ
		Vector3			move;		//�ړ���
        float           scale;      //�{�[���X�P�[��(���̃{�[���Ƃ̔���Ɏg����)
	};

	Params m_Params;

	//�R���X�g���N�^�E�f�X�g���N�^�Ŏ����I�Ƀ}�l�[�W���ɓo�^�E�폜���s��
	BallBase();

    //�J�E���^�[���s�����Ƃ��Ɏ����I�ɌĂ΂��֐�
    virtual void Counter(CharacterBase* pCounterCharacter){}

    //�{�[�������ł��ɍs�����Ƃ��Ɏ����I�ɌĂ΂��֐�(�߂�l�F���ł��������������ǂ���)
    virtual bool HitBall(BallBase* pBall){ return true; }

    bool isOutOfField()const;

	//�����̃{�[�����J�E���^�[�\���ǂ���
    static bool isCanCounter(const BallBase* pBall, CharacterBase* Me);

    //�ŗL�h�c�̃Q�b�^
    BallID GetID()const;

protected:
    virtual ~BallBase();

private:
    //�{�[���ŗL�h�c
    BallID m_ID;
};


//*****************************************************
//	�{�[���}�l�[�W��(�V���O���g������)
//*****************************************************
class BallManager
{
public:
    enum :size_t{ MaxBallNum = 30 };  //�{�[���������ɏo���ł���ő吔
    typedef std::array<BallBase*, MaxBallNum> BallArray;

	//�o�^�E�폜��BallBase�̂݉\�ɂ��邽��
	friend class BallBase;

	//�C���X�^���X�̃Q�b�^�E���
	static BallManager& GetInstance();
	static void Release();

	//�{�[���f�[�^�擾
  //  BallArray* GetBallData();

	//�����Ƃ��J�E���^�[����̂ɓK�����{�[���𓾂�
	bool GetCounterBall(
		BallBase**	ppOut,			//�߂�l�Ƃ��ĕԋp����{�[��
		CrVector3	character_pos,	//�L�����N�^�̏ꏊ
		Vector3*	pOutAfterFrameBallPos,//�L���b�`�܂ł̈ړ��t���[����̃{�[���̈ʒu
		float		catch_area_size,//�L���b�`�\�Ȕ͈�
		int			move_frame,		//�L���b�`�܂ł̈ړ��t���[��
        CharacterBase* Me           //�J�E���^�[����L�����N�^
		);

    //������ID�̃{�[�������݂��Ă��邩�ǂ���
    bool isBallEnable(BallBase::BallID Id);

private:
    enum :size_t
    {
        MaxBallRegistId = 256  //(1���E���h�Ɍ��Ă�ő�ʐ�)
    };
    typedef std::bitset<MaxBallRegistId> BallIDFlags;

	static BallManager*		             m_pInstance;
    BallArray				             m_BallData;
    BallIDFlags                          m_BallIDFlags;
    size_t                               m_IdSetCount;

	BallManager();
	~BallManager();

    BallBase::BallID    AddBall(BallBase* pBall);
	bool                EraceBall(BallBase* pBall);
};

#define DefBallMgr (BallManager::GetInstance())

#endif