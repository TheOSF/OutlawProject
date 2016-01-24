#ifndef __CHR_STATE_TEMPLATE_H__
#define __CHR_STATE_TEMPLATE_H__

//********************************************************
//		キャラクタ用ステートクラス
//********************************************************
template <class _Client,class Msg_t>
class ChrState
{
public:
	typedef _Client*_Client_type_ptr;
public:
	virtual ~ChrState(){}
	virtual void Enter(_Client_type_ptr){}
	virtual void Execute(_Client_type_ptr){}
	virtual void Exit(_Client_type_ptr){}

    virtual void StateMachineExit(_Client_type_ptr p){ Exit(p); }

	virtual bool Msg(Msg_t){ return false; }
};

//********************************************************
//		キャラクタ用ステートマシンクラス
//********************************************************
template <class _Client, class Msg_t>
class ChrStateMachine
{
public:
	typedef _Client*_Client_type_ptr;
	typedef ChrState<_Client, Msg_t>*_MyState_ptr;

    enum :int 
    {
        DONT_UPDATE_STATE_LEVEL = INT_MAX
    };

private:
	_Client_type_ptr		m_pClient;
	_MyState_ptr			m_pState;
	_MyState_ptr			m_pNextState;
    int                     m_ImportantLevel;
public:
    ChrStateMachine(_Client_type_ptr Client) :
        m_pState(0),
        m_pNextState(0),
        m_pClient(Client),
        m_ImportantLevel(0)
	{
	}
	virtual~ChrStateMachine()
	{
		if (m_pState)
		{
            m_pState->StateMachineExit(m_pClient);
			delete m_pState;
		}
		if (m_pNextState)delete m_pNextState;
	}
public:
    bool set_state(_MyState_ptr const Newstate, int ImportantLevel = 0)
	{
        if ( m_ImportantLevel == DONT_UPDATE_STATE_LEVEL || 
            m_ImportantLevel > ImportantLevel )
        {
            delete Newstate;
            return false;
        }

		if (m_pNextState)
		{
			delete m_pNextState;
		}

		m_pNextState = Newstate;
        m_ImportantLevel = ImportantLevel;
        return true;
	}
	void state_execute()
	{
		if (m_pNextState)
		{
			if (m_pState)
			{
				m_pState->Exit(m_pClient);
				delete m_pState;
			}
			m_pNextState->Enter(m_pClient);
			m_pState = m_pNextState;
			m_pNextState = nullptr;
            m_ImportantLevel = 0;
		}
		if (m_pState)
		{
			return m_pState->Execute(m_pClient);
		}
	}

	bool Msg(Msg_t t)
	{
		return m_pState->Msg(t);
	}
};
#endif