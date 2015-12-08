#include "Ball.h"
#include "debug\DebugFunction.h"

//*****************************************************
//	���x�[�X�N���X
//*****************************************************

const float BallBase::UsualBallShotY = 2.9f;

BallBase::BallBase()
{
    m_ID = DefBallMgr.AddBall(this);
}

BallBase::~BallBase()
{
    DefBallMgr.EraceBall(this);
}

bool BallBase::isOutOfField()const
{
    return m_Params.pos.Length() > 150.f;
}

bool BallBase::isCanCounter(const BallBase* pBall)
{
	//�J�E���^�[�\�ȃ{�[�����ǂ���
    return
        pBall->m_Params.type == BallBase::Type::_Usual ||
        pBall->m_Params.type == BallBase::Type::_Milder;
}

//�ŗL�h�c�̃Q�b�^
BallBase::BallID BallBase::GetID()const
{
    return m_ID;
}

//*****************************************************
//	�{�[���}�l�[�W��(�V���O���g������)
//*****************************************************
BallManager* BallManager::m_pInstance = nullptr;

BallManager& BallManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new BallManager();
	}

	return *m_pInstance;
}

void BallManager::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

////�{�[���f�[�^�擾
//BallManager::BallArray* BallManager::GetBallData()
//{
//	return &m_BallData;
//}

#define MyDebugStringC(str,...)\
{\
    char temp[512]; \
    sprintf_s<512>(temp, str, __VA_ARGS__); \
    OutputDebugString(temp); \
}

//�����Ƃ��J�E���^�[����̂ɓK�����{�[���𓾂�
bool BallManager::GetCounterBall(
	BallBase**	ppOut,			//�߂�l�Ƃ��ĕԋp����{�[��
	CrVector3	character_pos,	//�L�����N�^�̏ꏊ
	Vector3*	pOutAfterFrameBallPos,//�L���b�`�܂ł̈ړ��t���[����̃{�[���̈ʒu
	float		catch_area_size,//�L���b�`�\�Ȕ͈�
	int			move_frame		//�L���b�`�܂ł̈ړ��t���[��
	)
{
	
	float most_near = catch_area_size;		//�����΂�߂��{�[���܂ł̋���
	float temp_len;
	float Fmove_frame = (float)move_frame;	

	*ppOut = nullptr;

	Vector3 move_pos;
	Vector3 len;


    for (auto& it : m_BallData)
	{
        if (it == nullptr || !BallBase::isCanCounter(it))
		{
			continue;
		}

		//Fmove_frame��̃{�[���̈ʒu���Z�o
		move_pos = it->m_Params.move;
		move_pos *= Fmove_frame;
		move_pos += it->m_Params.pos;

		//�������Z�o
		len = move_pos - character_pos;
		temp_len = len.Length();

		//�ŒZ�������X�V�����ꍇ�͖߂�l�ɑ��
		if (most_near > temp_len)
		{
			most_near = temp_len;
            *ppOut = it;
			*pOutAfterFrameBallPos = move_pos;
		}
	}


	return *ppOut != nullptr;
}

bool BallManager::isBallEnable(BallBase::BallID Id)
{
    if (Id == BallBase::BallID::ErrorID)
    {
        return false;
    }

    return m_BallIDFlags.at((size_t)Id);
}


BallManager::BallManager() :
m_IdSetCount(0)
{
    m_BallData.fill(nullptr);
}

BallManager::~BallManager()
{

}

BallBase::BallID BallManager::AddBall(BallBase* pBall)
{
    size_t id = 0;
    //�󂫃C���f�b�N�X�ɒǉ�����
    for (size_t i = 0; i < MaxBallNum; ++i)
    {
        if (m_BallData[i] == nullptr)
        {
            m_BallData[i] = pBall;
            
            id = m_IdSetCount;

            MyAssert(m_BallIDFlags.at(m_IdSetCount) == false, "���łɎg���Ă���{�[���h�c���w�肳��܂���");
            
            m_BallIDFlags.at(m_IdSetCount) = true;

            m_IdSetCount = (m_IdSetCount + 1) % (size_t)MaxBallRegistId;

            MyAssert(m_IdSetCount < MaxBallRegistId, "�{�[���h�c�o�^��������ɒB���܂���");
            return (BallBase::BallID)id;
        }
    }

    MyAssert(
        false,
        "�{�[�������o�������ő�ɒB���܂��� �{�[������ %d ", (int)MaxBallNum
        );

    //���^���������I�H
    return BallBase::BallID::ErrorID;
}

bool BallManager::EraceBall(BallBase* pBall)
{
    const int id = (int)pBall->GetID();
    bool bit_reset = true;
    size_t i;

    MyAssert(id >= 0, "�{�[���폜���ł��܂���ł���");
    
    for (i = 0; i < MaxBallNum; ++i)
    {
        if (m_BallData[i] == pBall)
        {
            m_BallData[i] = nullptr;
            break;
        }
    }

    MyAssert(i < MaxBallNum, "�{�[���폜���ł��܂���ł����Q");
    MyAssert(m_BallIDFlags.at((size_t)id) == true, "�{�[���폜���ł��܂���ł����R");

    m_BallIDFlags.at((size_t)id) = false;
    
	return true;
}