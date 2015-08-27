#include "CharacterManager.h"
#include "CharacterFunction.h"


CharacterManager* CharacterManager::m_pInstance = nullptr;

CharacterManager& CharacterManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CharacterManager();
	}

	return *m_pInstance;
}

void CharacterManager::Release()
{
	delete m_pInstance;
	m_pInstance = nullptr;
}

//キャラクタデータのゲッタ
const CharacterManager::CharacterMap& 
	CharacterManager::GetCharacterMap()const
{
	return m_CharacterMap;
}

//キャラクタの生存人数を取得
UINT CharacterManager::GetCharacterLiveCount()const
{
	UINT ret = 0;

	for (auto it = m_CharacterMap.begin();
		it != m_CharacterMap.end();
		++it)
	{
		//死んでいなかった場合
		if (!chr_func::isDie(it->first))
		{
			++ret;
		}
	}

	return ret;
}

//ラウンド初期位置を得る
Vector3 CharacterManager::GetRoundStartPos(PlayerNum::Value num)const
{
    const Vector3 pos[4] =
    {
        Vector3(-20, 0, 20),
        Vector3(20, 0, 20),
        Vector3(-20, 0, -20),
        Vector3(20, 0, -20),
    };

    MyAssert(num >= 0 && num < 4, "不正なプレイヤー番号がCharacterManager::GetRoundStartPosに送られました num = %d ", (int)num);

    return pos[(int)num];
}

CharacterManager::CharacterManager()
{

}

CharacterManager::~CharacterManager()
{

}

void CharacterManager::Add(LpCharacterBase c)
{
	m_CharacterMap.insert(CharacterMap::value_type(c, c));
}

void CharacterManager::Erace(LpCharacterBase c)
{
	m_CharacterMap.erase(c);
}