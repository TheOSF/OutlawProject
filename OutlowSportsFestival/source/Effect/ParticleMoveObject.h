#ifndef __PARTICLE_MOVE_OBJECT_H__
#define __PARTICLE_MOVE_OBJECT_H__

#include "../GameSystem/GameObject.h"
#include "ParticleBase.h"

//-----------------------------------------------------------
// �p�[�e�B�N�����ړ��E�����Ǘ�������Q�[���I�u�W�F�N�g
//-----------------------------------------------------------

class ParticleMoveObject :public GameObjectBase
{
public:
   
    ParticleMoveObject(
        ParticleBase* pParticle,
        CrVector3     move,
        CrVector3     power,
        UINT          live_time,
        bool          animation,
        UINT          cellX,
        UINT          cellY
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

#endif