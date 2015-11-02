//
//
//          String.h
//
//


#pragma once


//
//     インクルードファイル
//
#include <string>
#include <sstream>
#include <tchar.h>


namespace org
{
	//
	//     [ String ]
	//     文字列を扱いやすくしたクラス
	//
	class String
	{
	public:
		using tstring = std::basic_string <TCHAR>;
		using tstringstream = std::basic_stringstream<TCHAR>;

		// コンストラクタ
		inline String();

		// コンストラクタ
		template<typename _Ty>
		inline String(const _Ty& val);

		// コピーコンストラクタ
		inline String(const String& str);

		template<typename _Ty>
		inline String& operator << (const _Ty& val);

		inline String& operator << (const String& str);

		inline operator const TCHAR* ();

		inline String& operator += (const String& str);

		inline String operator + (const String& str);

		inline const tstring& str()const;

	private:
		tstring string;
	};
}


//
//     [ String ]
//     文字列を扱いやすくしたクラス
//

// コンストラクタ
inline org::String::String() :
	string(_T(""))
{

}


// コンストラクタ
template<typename _Ty>
inline org::String::String(const _Ty& val) :
	string(_T(""))
{
	tstringstream ss;
	ss << val;
	string = ss.str();
}


// コピーコンストラクタ
inline org::String::String(const String& str) :
	string(str.string)
{

}


template<typename _Ty>
inline org::String& org::String::operator << (const _Ty& val)
{
	tstringstream ss;
	ss << val;
	string += ss.str();
	return *this;
}


inline org::String& org::String::operator << (const String& str)
{
	string += str.string;
	return *this;
}


inline org::String::operator const TCHAR* ()
{
	return string.c_str();
}


inline org::String& org::String::operator += (const String& str)
{
	string += str.string;
	return *this;
}


inline org::String org::String::operator + (const String& str)
{
	return String(string + str.string);
}


inline const org::String::tstring& org::String::str()const
{
	return string;
}
