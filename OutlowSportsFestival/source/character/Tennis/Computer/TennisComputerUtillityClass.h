#pragma once


#include "../TennisPlayer.h"
#include "../TennisState_Rolling.h"



class TennisComputerUtillityClass
{
public:

    class RollingCallBack :public TennisState_Rolling::CallBackClass
    {
    private:
        TennisPlayer* const m_pTennis;
        Vector3             m_EscapeVec;
    public:

        RollingCallBack(
            TennisPlayer*   pTennis,
            CrVector3       DamageVec
            ) :
            m_pTennis(pTennis)
        {
            //�������������Z�o
            Vector3 EscapeVec = Vector3Zero;

            Vector3Cross(EscapeVec, Vector3AxisY, DamageVec);

            EscapeVec.y = 0;
            EscapeVec.Normalize();

            //�����ō��E���]
            if (frand() < 0.5f)
            {
                EscapeVec = -EscapeVec;
            }

            //��������
            EscapeVec += Vector3Rand();

            EscapeVec.y = 0;
            EscapeVec.Normalize();

        }

        Vector3 GetVec()override
        {
            return m_EscapeVec;
        }

        void CancelUpdate()override
        {

        }
    };


};