#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include <map>
#include <list>
#include "debug\DebugFunction.h"

class GameObjectManager;


//********************************************************************
//	継承すると自動で更新するゲーム内オブジェクトのベースクラス
//********************************************************************

class GameObjectBase
{
public:
	enum MsgType
	{
		_PlayStart,	    //プレイ開始時(ラウンドごとの)に送られるメッセージ
		_RoundReset,	//ラウンド終了時に送られるメッセージ
        _TimeUp,        //タイムアップ時に送られるメッセージ
        _WinPose,       //勝利ポーズ実行時に呼ばれるメッセージ
	};

	GameObjectBase();

	virtual bool Update() = 0;	//falseを返すと消去する
	virtual bool Msg(MsgType mt) = 0;
protected:
	friend class GameObjectManager;

    virtual ~GameObjectBase();

#ifdef _DEBUG
private:
    bool    m_ManagerDelete;
#endif

};

typedef GameObjectBase* LpGameObjectBase;


//********************************************************************
//	オブジェクトクラスの描画・更新をするマネージャクラス
//********************************************************************

class GameObjectManager
{
public:
	static GameObjectManager& GetInstance();
	static void Release();
	
	void Update();	//全オブジェクト更新

    //全オブジェクトにメッセージを送信(戻り値：メッセージにtrueを返したオブジェクトの数)
	UINT SendMsg(
        GameObjectBase::MsgType mt  //送信するメッセージ
        );	
    
    //引数以外のオブジェクトの更新を引数のフレーム間行わない
    void FreezeOtherObjectUpdate(
        std::list<LpGameObjectBase> UpdateObjList,  //更新を行うオブジェクトリスト
        UINT OtherFreeze_frame                      //フリーズさせるフレーム
        );

private:

	typedef std::map<LpGameObjectBase, LpGameObjectBase> GameObjectMap;
    typedef std::list<LpGameObjectBase> GameObjectList;

	friend class GameObjectBase;

	static GameObjectManager*	m_pInstance;
	GameObjectMap				m_GameObjectMap;
    UINT                        m_FreezeFrame;
    GameObjectList              m_FreezeUpdateList;

	GameObjectManager();
	~GameObjectManager();

    void FreezeUpdate();
    void UsualUpdate();

	bool Add(LpGameObjectBase pObj);
	bool Erace(LpGameObjectBase pObj);
};

#define DefGameObjMgr (GameObjectManager::GetInstance())


#endif