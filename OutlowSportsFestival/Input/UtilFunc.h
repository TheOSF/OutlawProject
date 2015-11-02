//
//
//          UtilFunc.h
//
//


#pragma once


//
//     インクルードファイル
//
#include <array>
#include <vector>
#include <set>



// Com解放関数
template<class _Ty>
void SafeRelease(_Ty& p)
{
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}


// 配列内のポインタをdelete
template < class _Ty, size_t _Size >
void DeleteArrayPtr(std::array < _Ty, _Size > & v)
{
	for (auto& it : v)
	{
		delete it;
	}
	v.fill(nullptr);
}


// 配列内のポインタをdelete
template < class _Ty >
void DeleteVectorPtr(std::vector < _Ty > & v)
{
	for (auto& it : v)
	{
		delete it;
	}
	v.clear();
}


// 配列内のポインタをdelete
template < class _Ty >
void DeleteSetPtr(std::set < _Ty > & v)
{
	for (auto& it : v)
	{
		delete it;
	}
	v.clear();
}
