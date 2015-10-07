#pragma once

#include "GameObject.h"
#include "../character/CharacterManager.h"

//-------------------------------------------------------------------------------------//
//  環境光　と　平行光のイベント実行をするクラス(キャラクタ死亡時、夜に変わる　など)
//-------------------------------------------------------------------------------------//

class LightEventExecuter :public GameObjectBase
{
public:
    LightEventExecuter(
        LPVECTOR3 pDirLightColor,  //シーン終了時までdeleteしない前提
        LPVECTOR3 pAmbLightColor   //シーン終了時までdeleteしない前提
        );

    ~LightEventExecuter();

    bool Update();	//falseを返すと消去する
    bool Msg(MsgType mt);

private:

    LPVECTOR3 const m_pDirLightColor;
    LPVECTOR3 const m_pAmbLightColor;

    UINT      m_ChrLiveCount;

    Vector3   m_InitDir;
    Vector3   m_InitAmb;

    bool EventCheck();  //ライトオフのイベントをするかどうか
    void LightOff();    //ライトを暗くする
    void LightUpdate(); //ライト値を更新
};