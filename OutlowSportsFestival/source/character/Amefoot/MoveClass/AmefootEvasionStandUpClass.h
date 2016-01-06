#pragma once


class AmefootPlayer;
//-----------------------------------------------------------------------------------------
// AmefootEvasionStandUpClass
// [ �A���t�g ] ����㗧���オ��N���X
// �A���t�g��p�Ȃ̂ŃC�x���g���g�p���Ȃ�
//-----------------------------------------------------------------------------------------
class AmefootEvasionStandUpClass
{
public:
     struct Params
     {
          int AllFrame;
     };

     // �R���X�g���N�^
     AmefootEvasionStandUpClass(
          AmefootPlayer* pAmefoot,
          Params params);

     // �X�V
     bool Update();

private:
     AmefootPlayer* m_pAmefoot;
     const Params m_params;
     int m_timer;

};

