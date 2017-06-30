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
//	XStringT::String str2("hello");

	const char* A = NULL;
	const wchar_t* B = NULL;

	XStringT::IconvStringTranscoder tran;
	XStringT::String str3((XStringT::utf8 *)"123456 测试文本");
	
//	str3 = tran.stringFromStringW(std::wstring(L"123456 测试文本"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 測試文本"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 테스트 텍스트"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 テストテキスト"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 ทดสอบข้อความ"));
//	str3 = tran.stringFromStringW(std::wstring(L"123456 Kiểm tra văn bản"));

	A = tran.stringToANSI(str3);
	B = (const wchar_t*)tran.stringToUTF16(str3);

	return 0;
}
