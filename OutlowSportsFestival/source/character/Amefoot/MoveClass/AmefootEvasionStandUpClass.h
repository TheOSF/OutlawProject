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
         bool isViewAround;
     };

     // �R���X�g���N�^
     AmefootEvasionStandUpClass(
          AmefootPlayer* pAmefoot,
          Params params);

     // �X�V
     bool Update();

private:
    bool(AmefootEvasionStandUpClass::*m_pStateFunc)();

     AmefootPlayer* m_pAmefoot;
     int m_timer;

     bool State_ViewAround();
     bool State_StandUp();
};

