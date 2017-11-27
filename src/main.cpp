//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#if defined(_MSC_VER)
#include <windows.h>
#endif 

#include <string>
#include <iostream>

//
#include "MXStringT.h"

//
int main()
{
	MXString::String str;

	str = (MXString::utf8*)"123456";
	str += (MXString::utf8*)"abcdef";
	str += str +(MXString::utf8*) "00000";

	const MXString::utf32* cc = str.c_str();
	const MXString::utf8* cc8 = str.c_str_u8();

	MXString::StringX	str1 = "123456测试一下而已";
	str1 += "123456 測試文本";
	str1 = str1 + L"123456 테스트 텍스트";
	str1 = str1 + L"123456 テストテキスト";
	str1 = str1 + __MXSTRING_UNICODE_X("123456 ทดสอบข้อความ");
	const char* cca = str1.c_str_ua();
	const wchar_t* ccw = str1.c_str_uw();
	return 0;
}