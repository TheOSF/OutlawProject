#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "iextreme.h"

//***********************************************
//	コントローラーに関するヘッダ
//***********************************************

//前方宣言たち
struct ControllerButtonSet;

namespace controller
{
	typedef int CONTROLLER_NUM;	//コントローラ番号


    enum :CONTROLLER_NUM
    {
        __ERROR_CONTROLLER_NUM = -1  //コントローラ番号のエラー地値
    };

	namespace button
	{
		enum button_type
		{
			//ボタンタイプ
			maru,
			batu,
			shikaku,
			sankaku,

			//RLボタン
			_R1,
			_R2,
			_R3,
			_L1,
			_L2,
			_L3,

			//方向キータイプ
			up,
			down,
			right,
			left,

			max,//ボタンタイプ最大数
		};

		enum button_state
		{
			bs_up,
			bs_down,
			bs_trg,
		};
	}
	namespace stick{
		enum stick_type
		{
			//スティックタイプ
			left,
			right,

			max//スティックタイプ最大数
		};
	}
	//押した瞬間かどうか
    bool	GetTRG(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);
	//離した瞬間かどうか
	bool	GetLeave(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);
	//押しているかどうか
	bool	GetPush(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);

    //押しているかどうか(誰かが)
    bool	GetPushAnyController(button::button_type x);

    //押した瞬間かどうか(誰かが)
    bool	GetTRGAnyController(button::button_type x);

    //引数の値がスティックの誤差の値以内かどうか
    bool    isStickAdjustValue(float value);

	// コントローラの状態取得
	button::button_state GetButtonState(button::button_type x, CONTROLLER_NUM num, int* pCallCount = nullptr);

    //スティック(またはカーソル)を倒した瞬間かどうかを得る
    bool GetStickJustMove(stick::stick_type x, CONTROLLER_NUM num);

	//スティック(またはカーソル)の傾きの値を得る
	Vector2 GetStickValue(stick::stick_type x, CONTROLLER_NUM num);

	//指定したコントローラーを振動させる
    void SetVibration(
        DWORD power,       //-10000～10000
        float second,
        CONTROLLER_NUM num,
        int pattern = 0     //とりあえず０でおｋ
        );


    //更新(全体で１フレームに一回呼び出すだけ、使わないで！)
    void UpdateInfo();
}

//デフォルトのbuttonセットを設定する
void SetUsualButtonSet(ControllerButtonSet& out, controller::CONTROLLER_NUM number);


#endif