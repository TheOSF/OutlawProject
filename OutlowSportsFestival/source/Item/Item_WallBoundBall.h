#pragma once


#include "../Ball/Ball.h"
#include "../Damage/Damage.h"
#include "../utillity/LocusHDR.h"
#include "../Render/MeshRenderer.h"
#include "../GameSystem/GameObject.h"

class Item_Bell;

//***************************************************
//		ベル鳴らし用ボールクラス
//***************************************************

class Item_WallBoundBall :public GameObjectBase, public BallBase
{
public:
    //コンストラク
    Item_WallBoundBall(CrVector3 pos, CrVector3 move, CrVector3 target, Item_Bell* pBell);
    ~Item_WallBoundBall();

    void Destroy();

    bool Update();
    bool Msg(MsgType mt);

private:
    CharacterBase*      m_pHitCharacter;
    Item_Bell* const    m_pBell;
    MeshRenderer		m_MeshRenderer;
    LocusHDR            m_Locus;
    bool                m_LiveFlg;
    const Vector3       m_Target;
    const float         m_Glavity;
    float               m_ColorTimer;
    int                 m_Timer;

    void(Item_WallBoundBall::*m_pStateFunc)();

    Vector3 GetToMoveValue()const;   //鐘への移動ベクトルを算出する
    Vector4 GetColor(RATIO t)const;  //tに応じた虹色カラーを算出
    void UpdateLocusColor();         //色更新

    void ApperEffect();              //エフェクト
    void UpdateMesh();               //メッシュ更新
    bool WallCheck(Vector3* pNormal);//壁との判定を取り,trueを返す場合にはpNormalに壁法線を入れている

    void State_ToFloorMove();
    void State_RefrectMove();
    void State_ToBellMove();
    void State_Finish();

    void Counter(CharacterBase* pCounterCharacter)override;
};