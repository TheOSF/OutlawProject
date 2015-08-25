#ifndef __GAME_INITIALIZER_H__
#define __GAME_INITIALIZER_H__

//------------------------------------------------------------
//  �Q�[���������N���X�C���^�[�t�F�[�X
//------------------------------------------------------------

class IGameInitializer
{
public:
    virtual ~IGameInitializer(){}

    virtual void GameCreate() = 0;     //�Q�[���J�n���ɌĂ΂��
    virtual void GameRoundReset() = 0; //���E���h���Z�b�g���ɌĂ΂��
};


#endif