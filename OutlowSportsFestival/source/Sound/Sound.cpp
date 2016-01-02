#include "Sound.h"

#define SoundPath "DATA\\sound\\"
#define RegistSE(Type,Name) (IEX_SetWAV((int)Type,SoundPath"SE\\"Name".wav"))
#define RegistBGM(Type,Name) (IEX_SetWAV((int)Type,SoundPath"BGM\\"Name".wav"))


void Sound::Initialize()
{
    RegistSE(Damage1, "Damage1");
    RegistSE(Damage2, "Damage2");
    RegistSE(Damage3, "Damage3");

    RegistSE(AtkHit1, "AtkHit1");
    RegistSE(AtkHit2, "AtkHit2");
    
    RegistSE(Swing1, "Swing1");
    RegistSE(Swing2, "Swing2");
    RegistSE(Swing3, "Swing3");
    
    
    RegistSE(Impact1, "Impact1");
    RegistSE(Impact2, "Impact2");
    

    RegistSE(Sand1, "Sand1");
    RegistSE(Sand2, "Sand2");

    RegistSE(Counter, "Counter");

    RegistSE(Explode, "Explode");
    RegistSE(Explode2, "Explode2");

    RegistSE(Skill, "Skill");

    RegistSE(Beam1, "Beam");
    RegistSE(Beam2, "Beam2");

    RegistSE(Tennis_BallAtk, "tennis\\tenis_hitball");

    RegistSE(BaseBall_kaki_n, "BaseBall_kaki_normal");
	RegistSE(BaseBall_SP, "BaseBall_kaki_n");

    RegistSE(Golf_Swing, "golfu_swing");
    

    RegistSE(Cursor_move, "Button\\cursor");
    RegistSE(Cursor_enter, "Button\\Enter");
    RegistSE(Cursor_cancel, "Button\\kyanseru");
    
    RegistSE(Kira_n, "kira-n");

    RegistSE(Gong_Start, "gong_start");
    RegistSE(Gong_End, "gong_end");

    RegistSE(Scene_Enter, "enter_next");

	RegistSE(Change, "change");

	RegistSE(Soccer_Brake, "brake");

	RegistSE(Soccer_Speed_Up1, "speedUp1");
	RegistSE(Soccer_Speed_Up2, "speedUp2");

    RegistBGM(BGM_Title, "title\\game_maoudamashii_4_vehicle02");
    RegistBGM(BGM_Option, "option\\select2");
    RegistBGM(BGM_ChrSelect, "character_select\\select");

    RegistBGM(BGM_Battle,"battle\\battle_normal");
    RegistBGM(BGM_LastRound, "battle\\battle_last");

}


void Sound::Play(Type type, RATIO volume , bool loop)
{
   // return;  //これ！！！

    IEX_PlaySound(
        (int)type,
        loop
        );
}


//ＢＧＭストップ
void Sound::StopBGM()
{
    for (int i = (int)_BGM_REGIST_START + 1; i < (int)_BGM_REGIST_END; ++i)
    {
        IEX_StopSound(i);
    }
}