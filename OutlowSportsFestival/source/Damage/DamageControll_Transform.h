#pragma once

//-----------------------------------------------------------------------//
// �@�L�����N�^����������I�u�W�F�N�g
//-----------------------------------------------------------------------//

#include "../GameSystem/GameObject.h"
#include "../character/CharacterDamageControll.h"
#include <array>

class DamageControll_Transform :public GameObjectBase
{
public:

    bool    m_Destroy;       //�폜���邩�ǂ���(���̃N���X��)(���̃t���O��true�ɂ���ȊO�ł��̃N���X�͍폜����Ȃ�)
    Matrix  m_Transform;     //�p���s��(�L�����N�^����)

    DamageControll_Transform();
    ~DamageControll_Transform();

    //�t���Ă���L�����N�^�����ׂĕ���(�f�X�g���N�^�Ŏ����I�ɌĂ΂��
    void AllFree();

    //���삷��L�����N�^��ǉ�����
    void AddControllClass(CharacterDamageControll* pControllClass);

    //�t���Ă��邷�ׂẴL�����N�^�Ƀ_���[�W��^����(�����F�P�@�_���[�W�ʁ@�F�Q�@�̗͂��P�c�����ǂ���)
    void AddDamage(float value, bool dontDie = false);

private:
    std::array<CharacterDamageControll*, 4> m_ChrControllArray;

    std::array<CharacterBase*, 4> m_ChrArray;  //

    bool Update();
    bool Msg(MsgType mt);
};

