
#pragma once

#include "iextreme.h"
#define		DIRECTINPUT_VERSION	0x0800 
#include	<dinput.h>


//入力方法切り替え

#if TRUE

#define OUTLAW2_CONTROLL_TYPE_IEX_INPUT

#else

#define OUTLAW2_CONTROLL_TYPE_CONTROLLER

#endif

//*****************************************************************************************************************************
//
//		入力関連
//
//*****************************************************************************************************************************

//*****************************************************************************
//		入力デバイス管理
//*****************************************************************************
class iexInputManager
{
private:
     static LPDIRECTINPUT8 pDI;

     static int NumDevice;
     static DIDEVICEINSTANCE	didi[4];

     static BOOL CALLBACK EnumDeviceCallback(const DIDEVICEINSTANCE* pdidi, VOID* pContext);
     static BOOL CALLBACK EnumAxes(LPCDIDEVICEOBJECTINSTANCE lpddoi, LPVOID pvRef);

public:
     static void Initialize();
     static LPDIRECTINPUTDEVICE8 GetDevice(int n);

     static void Release()
     {
          if ( pDI ) pDI->Release();
     }
};


typedef u8	KEYCODE, *LPKEYCODE;

#define	KEY_UP		0
#define	KEY_DOWN	1
#define	KEY_LEFT	2
#define	KEY_RIGHT	3

#define	KEY_A		4
#define	KEY_B		5
#define	KEY_C		6
#define	KEY_D		7
#define	KEY_X		6
#define	KEY_Y		7

#define	KEY_L		8
#define	KEY_L1		8
#define	KEY_L2		10
#define	KEY_L3		12

#define	KEY_R		9
#define	KEY_R1		9
#define	KEY_R2		11
#define	KEY_R3		13

#define	KEY_START	14
#define	KEY_SELECT	15
#define	KEY_BACK	15

#define	KEY_ENTER	14
#define	KEY_SPACE	15

#define	KEY_B1		4
#define	KEY_B2		5
#define	KEY_B3		6
#define	KEY_B4		7
#define	KEY_B5		8
#define	KEY_B6		9
#define	KEY_B7		10
#define	KEY_B8		11
#define	KEY_B9		12
#define	KEY_B10		13
#define	KEY_B11		14
#define	KEY_B12		15
#define	KEY_B13		16
#define	KEY_B14		17
#define	KEY_B15		18

#define	KEY_AXISX	200
#define	KEY_AXISY	201
#define	KEY_AXISX2	202
#define	KEY_AXISY2	203

typedef struct tagKEYSET
{
     u8	up, down, left, right;
     u8	A, B, X, Y;
     u8	L1, L2, L3;
     u8	R1, R2, R3;
     u8	START, SELECT;
} KEYSET, *LPKEYSET;


#define	AXIS_X	0
#define	AXIS_Y	1
#define	AXIS_Z	2
#define	AXIS_RX	3
#define	AXIS_RY	4
#define	AXIS_RZ	5

typedef struct tagPADSET
{
     u8	lx, ly, rx, ry;
     u8	A, B, X, Y;
     u8	L1, L2, L3;
     u8	R1, R2, R3;
     u8	START, SELECT;
} PADSET, *LPPADSET;

class iexInput
{
private:
     LPDIRECTINPUTDEVICE8 lpDevice;
     LPDIRECTINPUTEFFECT	 pEffect;

     static const int OrgKeyMap[20];
     static const int OrgJoyMap[20];
     int KeyMap[20];
     int JoyMap[20];
     u8	KeyInfo[20], JoyInfo[20];
     u8 CallCounter[20][4];
     int		PadAxisX, PadAxisY;
     int		PadAxisX2, PadAxisY2;

     BOOL InitVibration();


public:
     static PADSET	ps101;
     static PADSET	sixaxis;
     static PADSET	xbox360;

     iexInput(int n);
     ~iexInput();

     void Asign(KEYSET& keyset);
     void PadAsign(PADSET& padset);

     void SetInfo();
     int Get(KEYCODE key, int* pCallCount = nullptr);

     void Vibration(u32 gain, float period);
};


//*****************************************************************************
//		アクセス関数
//*****************************************************************************
void	KEY_Asign(KEYSET& padset, int n);
void	KEY_PadAsign(PADSET& padset, int n);

void	KEY_SetInfo(int n);
int		KEY_Get(KEYCODE key, int n, int* pCallCount = nullptr);
void	KEY_Vibration(u32 gain, float period, int n);

int		KEY_GetAxisX(int n);
int		KEY_GetAxisY(int n);
int		KEY_GetAxisX2(int n);
int		KEY_GetAxisY2(int n);

BOOL	IEX_InitInput();
void	IEX_ReleaseInput();

#define	KEY(x,n)	KEY_Get(x,n,nullptr)

inline bool GetAnyKey(KEYCODE key)
{
    for ( int i = 0; i < 4; i++ )
        if ( KEY(key, i) == 3 )return true;
     return false;
}

