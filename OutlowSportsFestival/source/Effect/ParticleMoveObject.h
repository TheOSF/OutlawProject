#ifndef __PARTICLE_MOVE_OBJECT_H__
#define __PARTICLE_MOVE_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "ParticleBase.h"

//-----------------------------------------------------------
// パーティクルを移動・寿命管理をするゲームオブジェクト
//-----------------------------------------------------------

class ParticleMoveObject :public GameObjectBase
{
public:
   
    ParticleMoveObject(
        ParticleBase* pParticle,   //動かすパーティクルへのポインタ
        CrVector3     move,        //移動量
        CrVector3     power,       //力場(重力とか)
        UINT          live_time,   //消滅するまでの時間
        bool          animation,   //テクスチャアニメーションをするかどうか
        UINT          cellX,       //テクスチャのコマ数横(アニメーションしない場合は１を設定してください)
        UINT          cellY        //テクスチャのコマ数縦(アニメーションしない場合は１を設定してください)
        );

    ~ParticleMoveObject();


    bool Update();
    bool Msg(MsgType mt);
    
private:
    ParticleBase*const   pParticle;
    Vector3              move;
    const Vector3        power;
    const COLORf         start_color;
    const UINT           delete_time;
    UINT                 time;
    const  bool          animation;
    const  UINT          cellX, cellY;
};



//色の管理ができるver
class ParticleMoveObject2 :public GameObjectBase
{
public:

    ParticleMoveObject2(
        ParticleBase* pParticle,   //動かすパーティクルへのポインタ
        CrVector3     move,        //移動量
        CrVector3     power,       //力場(重力とか)
        bool          animation,   //テクスチャアニメーションをするかどうか
        UINT          cellX,       //テクスチャのコマ数横(アニメーションしない場合は１を設定してください)
        UINT          cellY,        //テクスチャのコマ数縦(アニメーションしない場合は１を設定してください)
        
        UINT          md_frame,
        UINT          end_frame,

        DWORD         md_color,
        DWORD         end_color
        );

    ~ParticleMoveObject2();


    bool Update();
    bool Msg(MsgType mt);

private:
    ParticleBase*const   pParticle;
    Vector3              move;
    const Vector3        power;

    UINT                 time;
    const  bool          animation;
    const  UINT          cellX, cellY;

    const DWORD          start_color;
    const DWORD          md_color;
    const DWORD          end_color;

    const UINT           md_frame;
    const UINT           end_frame;

};


#endif