#ifndef __GAME_INITIALIZER_H__
#define __GAME_INITIALIZER_H__

//------------------------------------------------------------
//  ゲーム初期化クラスインターフェース
//------------------------------------------------------------

class IGameInitializer
{
public:
    virtual ~IGameInitializer(){}

    virtual void GameCreate() = 0;     //ゲーム開始時に呼ばれる
    virtual void GameRoundReset() = 0; //ラウンドリセット時に呼ばれる
};


#endif