#ifndef __CHARACTER_RENDERER_H__
#define __CHARACTER_RENDERER_H__

#include "../Render/Renderer.h"
#include "../brendanimation/BlendAnimationMesh.h"
#include "../utillity/ColorUtility.h"
#include <vector>

//*************************************************
//	キャラクタ用メッシュクラス
//*************************************************

class CharacterRenderer :public DeferredRenderer, public ForwardHDRRenderer
{
public:
    enum class RenderType
    {
        Skin,
        Hair,
        Clothes,
        Face,
        Normal,
    };

	//通常コンストラクタ
	CharacterRenderer(
		BlendAnimationMesh* pAnimeMesh
		);

	//部位モーションをする際に
	CharacterRenderer(
		BlendAnimationMesh*		pAnimeMesh,
		const std::vector<int>& UpBodyBoneNums,
		const std::vector<int>& DownBodyBoneNums
		);

	~CharacterRenderer();


    //マテリアルがどの部位かを指定する(描画切り替えのため)
    void SetMaterialRenderType(int MaterialNum, RenderType Type);


	//通常のモーションセット
	void SetMotion(int m);

	//上半身・下半身ごとのボーションセット
	void SetBodyUpMotion(int m);
	void SetBodyDownMotion(int m);

	//通常のモーションスピードセット
	void SetMotionSpeed(float t);

	//上半身・下半身ごとのスピードセット
	void SetBodyUpMotionSpeed(float t);
	void SetBodyDownMotionSpeed(float t);

	//モーション更新
	void Update(float t);

    //ワールド空間上でのボーン行列を得る
    void GetWorldBoneMatirx(Matrix& Out, int BoneNum);

    //ローカル空間(キャラクタ空間)上でのボーン行列を得る
    void GetLocalBoneMatrix(Matrix& Out, int BoneNum);

    //ワールド空間上でのボーン座標を得る
    Vector3 GetWorldBonePos(int BoneNum);

    //ローカル空間(キャラクタ空間)上でのボーン座標を得る
    Vector3 GetLocalBonePos(int BoneNum);

	//ワールド変換行列
	Matrix  m_TransMatrix;

    //体全体の輝き度(０が標準、xyz = rgb)
    Vector3 m_HDR;

    //ライト値を加算する
    Vector3 m_Lighting;

    //服の色（xyz = rgb)
    Vector3 m_ClothesColor;

    //輪郭線を描画するかどうか
    bool    m_OutlineVisible;

    //輪郭線の色(１以上の値は光る、２が最大値)
    COLORf m_OutlineColor;


    void DepthRender(iexShader* pShader, const char* pTec, DepthRenderType type);

private:
	typedef std::vector<int> BoneNumArray;
    typedef std::map<int, const char*> Techniques;

	BlendAnimationMesh* m_pAnimeMesh;
	const bool m_UsePartsMotion;

	const BoneNumArray m_UpBodyBoneNumArray;
	const BoneNumArray m_DownBodyBoneNumArray;

	int m_BodyUpMotion;
	int m_BodyDownMotion;

	float m_BodyUpMotionSpeed;
	float m_BodyDownMotionSpeed;

    Techniques m_Techniques;

	void Initialize();

    void GbufRender(
        iexShader*        pShader,                       //シェーダークラス
        DeferredGbufRenderer::TechniqueSetter*  pSetter  //テクニック管理クラス
        );

    void MasterRender();


    void CalcZ();
    void Render();
};

#endif