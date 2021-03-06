#pragma once
#include "../GameSystem/ForwardDecl.h"
#include "../utillity/ColorUtility.h"
#include "../GameSystem/GameSystem.h"

//---------------------------------------------//
// 　　　エフェクト出現関数集
//---------------------------------------------//

namespace EffectFactory
{
    //煙アニメーションエフェクト
    void Smoke(
        CrVector3 pos,
        CrVector3 move,
        float size,
        float alpha,
        bool Soft = false);

    //簡単にそれっぽいのがでる
    void SmokeInstant(
        CrVector3 pos,
        CrVector3 move,
        float size,
        int live_frame
        );

    //煙エフェクト
    void SmokeParticle(
        CrVector3 pos,
        CrVector3 move,
        UINT   live_frame,
        float size,
        DWORD Color,
        bool Soft = false);

    //カウンターのアニメーション
    void Counter(
        CrVector3 pos,
        float size);

	//　切り替え
	void Change(
        CrVector3 pos
		);

    //円が大きくなるアニメーション
    void CircleAnimationBillbord(
        CrVector3   pos,
        CrVector3   move,
        CrVector3   power,
        CrVector2   size,
        DWORD       color,
        DWORD       dw_flag
        );

    //円が大きくなるアニメーション
    void CircleAnimation(
        CrVector3   pos,
        CrVector3   vec,
        CrVector3   move,
        CrVector3   power,
        CrVector2   size,
        DWORD       color,
        DWORD       hdr_col,
        RATIO       speed = 1.0f
        );

    //パーティクル
	void Particle(
		UINT      type,
		UINT      live_time,
		CrVector3 pos,
		CrVector3 move,
		CrVector2 size,
		DWORD     Color,
		bool Soft = false,
		DWORD dw_Type = RS_COPY
        );

	//パーティクル
	void ParticleHDR(
		UINT      type,
		UINT      live_time,
		CrVector3 pos,
		CrVector3 move,
		CrVector2 size,
		DWORD     Color
		);

    //ショット時のエフェクト
    void ShotEffect(
        PlayerNum::Value    num,
        CrVector3           pos, 
        CrVector3           vec
        );

    //攻撃ヒット時のエフェクト
    void HitEffect(
        CharacterBase* pHitCharacter,
        CrVector3      Damage_Vec
        );

    //吹き飛び攻撃くらい時のエフェクト
    void VanishEffect(
        CharacterBase* pHitCharacter,
        CrVector3      Damage_Vec
        );

    //死亡時のエフェクト
    void DieEffect(
        CharacterBase* pDieCharacter,
        CrVector3      Damage_Vec
        );


    //軌跡パーティクル
    void LocusParticle(
        CrVector3 pos,
        CrVector3 move,
        CrVector3 power,
        float     width,
        UINT      length,
        COLORf    color,
        COLORf    HDRcolor,
        UINT      live_frame,
        RATIO     bound = 0.2f,
        RATIO     speedDown = 0.0f
        );
}