#ifndef __BALL_H__
#define __BALL_H__

#include <array>
#include <list>
#include <bitset>
#include "iextreme.h"
#include "../GameSystem/ForwardDecl.h"

//*****************************************************
//		玉ベースクラス
//*****************************************************

class BallBase
{
public:
	//通常ボール打ちのワールド座標上でのyの値
	static const float UsualBallShotY;

    //ボール固有ID
    enum BallID :int
    {
        ErrorID = -1 //エラー値
    };

	//球の機能のタイプ
	enum Type
	{
		_DontWork,				//描画はしているが、あたり判定のないボール
		_Usual,					//通常のボール
        _ICantCounter,          //打ったプレイヤーのみカウンターできない
		_CantCounter,			//カウンターできない通常のボール
        _Milder,                //野球ホーミング
		_BaseballSpecialAtk     //野球必殺
	};

	//パラメータ構造体
	struct Params
	{
		CharacterBase*	pParent;	//発射したキャラクタへのポインタ
		Type			type;		//玉のタイプ
		Vector3			pos;		//場所
		Vector3			move;		//移動量
        float           scale;      //ボールスケール(他のボールとの判定に使われる)
	};

	Params m_Params;

	//コンストラクタ・デストラクタで自動的にマネージャに登録・削除を行う
	BallBase();

    //カウンターを行ったときに自動的に呼ばれる関数
    virtual void Counter(CharacterBase* pCounterCharacter){}

    //ボールが相打ちに行ったときに自動的に呼ばれる関数(戻り値：相打ちが成功したかどうか)
    virtual bool HitBall(BallBase* pBall){ return true; }

    bool isOutOfField()const;

	//引数のボールがカウンター可能かどうか
    static bool isCanCounter(const BallBase* pBall, CharacterBase* Me);

    //固有ＩＤのゲッタ
    BallID GetID()const;

protected:
    virtual ~BallBase();

private:
    //ボール固有ＩＤ
    BallID m_ID;
};


//*****************************************************
//	ボールマネージャ(シングルトン実装)
//*****************************************************
class BallManager
{
public:
    enum :size_t{ MaxBallNum = 30 };  //ボールが同時に出現できる最大数
    typedef std::array<BallBase*, MaxBallNum> BallArray;

	//登録・削除をBallBaseのみ可能にするため
	friend class BallBase;

	//インスタンスのゲッタ・解放
	static BallManager& GetInstance();
	static void Release();

	//ボールデータ取得
  //  BallArray* GetBallData();

	//もっともカウンターするのに適したボールを得る
	bool GetCounterBall(
		BallBase**	ppOut,			//戻り値として返却するボール
		CrVector3	character_pos,	//キャラクタの場所
		Vector3*	pOutAfterFrameBallPos,//キャッチまでの移動フレーム後のボールの位置
		float		catch_area_size,//キャッチ可能な範囲
		int			move_frame,		//キャッチまでの移動フレーム
        CharacterBase* Me           //カウンターするキャラクタ
		);

    //引数のIDのボールが存在しているかどうか
    bool isBallEnable(BallBase::BallID Id);

private:
    enum :size_t
    {
        MaxBallRegistId = 256  //(1ラウンドに撃てる最大玉数)
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