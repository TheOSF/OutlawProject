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
        ParticleBase* pParticle,   //�������p�[�e�B�N���ւ̃|�C���^
        CrVector3     move,        //�ړ���
        CrVector3     power,       //�͏�(�d�͂Ƃ�)
        UINT          live_time,   //���ł���܂ł̎���
        bool          animation,   //�e�N�X�`���A�j���[�V���������邩�ǂ���
        UINT          cellX,       //�e�N�X�`���̃R�}����(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)
        UINT          cellY        //�e�N�X�`���̃R�}���c(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)
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



//�F�̊Ǘ����ł���ver
class ParticleMoveObject2 :public GameObjectBase
{
public:

    ParticleMoveObject2(
        ParticleBase* pParticle,   //�������p�[�e�B�N���ւ̃|�C���^
        CrVector3     move,        //�ړ���
        CrVector3     power,       //�͏�(�d�͂Ƃ�)
        bool          animation,   //�e�N�X�`���A�j���[�V���������邩�ǂ���
        UINT          cellX,       //�e�N�X�`���̃R�}����(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)
        UINT          cellY,        //�e�N�X�`���̃R�}���c(�A�j���[�V�������Ȃ��ꍇ�͂P��ݒ肵�Ă�������)
        
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