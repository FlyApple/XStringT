//============================================================================
// Name        : XStringT.cpp
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

#include "XStringT.h"
#include "XStringTranscoderT.h"

//
int main()
{
	XStringT::String str1(std::string("hello"));
	XStringT::String str2("hello");

	const char* A = str2.c_str();
	str1 = str1 + " " + "world";
	const char* B = str1.c_str();

	XStringT::IconvStringTranscoder tran;
	XStringT::String str3 = tran.stringFromStringW(std::wstring(L"123456 测试文本"));
	std::wstring strW = tran.stringToStringW(str3);
	
//	str3 = tran.stringFromStringW(std::wstring(L"123456 測試文本"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 테스트 텍스트"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 テストテキスト"));
	A = tran.stringToANSI(str3);
	return 0;
}
