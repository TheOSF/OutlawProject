#ifndef __GAME_EVENTER_H__
#define __GAME_EVENTER_H__

#include "GameObject.h"
#include "../utillity/StateTemplate.h"
#include "ForwardDecl.h"
#include "../character/CharacterManager.h"

//----------------------------------------------------
//  �����J�ڃ��b�Z�[�W�𑗐M����N���X
//----------------------------------------------------

class GameEventer :public GameObjectBase
{
public:
	typedef State<GameEventer, void> State;
	typedef StateMachine<GameEventer, void> StateMachine;

	struct Param
	{
		UINT  round;
		UINT  time;

        UINT  round_count;
	};

	Param          m_Param;
    
    GameEventer(const Param& p, State* pInitState, LPVECTOR3 pControllDirColor);
	~GameEventer();
	
	void SetState(State* pState);
    void SetLightChange();

private:

	StateMachine*  m_pStateMachine;
    Vector3* const m_pControllDirColor;
    const Vector3  m_InitDirColor;


	bool Update();
	bool Msg(MsgType mt);

};


//----------------------------------------------------
//  �X�e�[�g�N���X
//----------------------------------------------------

namespace MatchState
{
	//�����J�n�̃J�E���g�_�E��
	class StartCountdown :public GameEventer::State
	{
	public:
		StartCountdown();
		~StartCountdown();

	private:
		UINT m_Timer;
		UINT m_MaxTime;

		void SetCamera();

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);
	};

    //�Q���E���h�ڈȍ~�̊J�n�X�e�[�g
    class RoundResetCountdown :public GameEventer::State
    {
    public:
        RoundResetCountdown();
        ~RoundResetCountdown();

    private:
        UINT m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

	//�������̃X�e�[�g
	class MatchPlay :public GameEventer::State
	{
	public:
		MatchPlay();
		~MatchPlay();

	private:

        //�Q�[���X�g�b�v���Ǘ�����N���X
        class GameStopMgr
        {
        public:
            GameStopMgr();

            //���t���[���̍X�V
            void Update();

            //�X�g�b�v������(�����F���t���[����ɃX�g�b�v���邩�A���t���[���X�g�b�v���邩)
            void SetStop(UINT ep_frame, UINT stop_frame, CharacterBase* pUpdateChr=nullptr);
        private:
            int m_Ep_frame;
            int m_Stop_frame;
            CharacterBase* m_pUpdateChr;
        };

        CharacterManager::CharacterMap  m_CharacterMap;
		UINT                            m_Frame;
        UINT                            m_PreLiveCount;
        GameStopMgr                     m_GameStopMgr;

        void GetLiveCharacterMap(CharacterManager::CharacterMap& Out);

		void Enter(_Client_type_ptr);
		void Execute(_Client_type_ptr);
		void Exit(_Client_type_ptr);

        void SwitchState(const UINT now_LiveCount, _Client_type_ptr p);
        CharacterBase* GetNowDieCharacter();
	};

    //��l�����X�e�[�g
    class WinPose :public GameEventer::State
    {
    public:
        WinPose(
            LpCharacterBase pLastDieCharacter,
            LpCharacterBase pWinCharacter
            );
        ~WinPose();

    private:
    
        LpCharacterBase  m_pLastDieCharacter;
        LpCharacterBase  m_pWinCharacter;
        UINT             m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

    //�^�C���A�b�v�X�e�[�g
    class TimeUp :public GameEventer::State
    {
    public:
        TimeUp(LpCharacterBase pWinCharacter);

    private:
        UINT             m_Frame;
        LpCharacterBase  m_pWinCharacter;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };


    //���������X�e�[�g
    class Draw :public GameEventer::State
    {
    private:
        UINT             m_Frame;
        LpCharacterBase  m_pWinCharacter;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };

    //���E���h���Z�b�g�X�e�[�g
    class ResetRound :public GameEventer::State
    {
    private:
        UINT             m_Frame;

        void Enter(_Client_type_ptr);
        void Execute(_Client_type_ptr);
        void Exit(_Client_type_ptr);
    };
}

#endif