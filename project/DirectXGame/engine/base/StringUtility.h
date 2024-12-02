#pragma once
#include<string>
#include <windows.h>

// 文字コードユーティリティ
namespace StringUtility 
{
	//std::wstringからstd::stringへ変換する関数
	//string->wstring
	std::wstring ConvertString(const std::string& str);
	//wstring->string
	std::string ConvertString(const std::wstring& str);


}

