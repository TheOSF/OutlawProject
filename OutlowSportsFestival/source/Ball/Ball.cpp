#include "Ball.h"
#include "debug\DebugFunction.h"

//*****************************************************
//	球ベースクラス
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
	//カウンター可能なボールかどうか
    return
        pBall->m_Params.type == BallBase::Type::_Usual ||
        pBall->m_Params.type == BallBase::Type::_Milder;
}

//固有ＩＤのゲッタ
BallBase::BallID BallBase::GetID()const
{
    return m_ID;
}

//*****************************************************
//	ボールマネージャ(シングルトン実装)
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

////ボールデータ取得
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

//もっともカウンターするのに適したボールを得る
bool BallManager::GetCounterBall(
	BallBase**	ppOut,			//戻り値として返却するボール
	CrVector3	character_pos,	//キャラクタの場所
	Vector3*	pOutAfterFrameBallPos,//キャッチまでの移動フレーム後のボールの位置
	float		catch_area_size,//キャッチ可能な範囲
	int			move_frame		//キャッチまでの移動フレーム
	)
{
	
	float most_near = catch_area_size;		//いちばん近いボールまでの距離
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

		//Fmove_frame後のボールの位置を算出
		move_pos = it->m_Params.move;
		move_pos *= Fmove_frame;
		move_pos += it->m_Params.pos;

		//距離を算出
		len = move_pos - character_pos;
		temp_len = len.Length();

		//最短距離を更新した場合は戻り値に代入
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
    //空きインデックスに追加する
    for (size_t i = 0; i < MaxBallNum; ++i)
    {
        if (m_BallData[i] == nullptr)
        {
            m_BallData[i] = pBall;
            
            id = m_IdSetCount;

            MyAssert(m_BallIDFlags.at(m_IdSetCount) == false, "すでに使われているボールＩＤが指定されました");
            
            m_BallIDFlags.at(m_IdSetCount) = true;

            m_IdSetCount = (m_IdSetCount + 1) % (size_t)MaxBallRegistId;

            MyAssert(m_IdSetCount < MaxBallRegistId, "ボールＩＤ登録数が上限に達しました");
            return (BallBase::BallID)id;
        }
    }

    MyAssert(
        false,
        "ボール同時出現数が最大に達しました ボール数＝ %d ", (int)MaxBallNum
        );

    //満タンだった！？
    return BallBase::BallID::ErrorID;
}

bool BallManager::EraceBall(BallBase* pBall)
{
    const int id = (int)pBall->GetID();
    bool bit_reset = true;
    size_t i;

    MyAssert(id >= 0, "ボール削除ができませんでした");
    
    for (i = 0; i < MaxBallNum; ++i)
    {
        if (m_BallData[i] == pBall)
        {
            m_BallData[i] = nullptr;
            break;
        }
    }

    MyAssert(i < MaxBallNum, "ボール削除ができませんでした２");
    MyAssert(m_BallIDFlags.at((size_t)id) == true, "ボール削除ができませんでした３");

    m_BallIDFlags.at((size_t)id) = false;
    
	return true;
}