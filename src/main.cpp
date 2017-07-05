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
#include "XStringSwapperT.h"

//
int main()
{
//	XStringT::StringAU8 str((XStringT::utf8*)"!123456 hello world! 测试文本");
//	str += (XStringT::utf8*)"啊咧咧，啊哈哈！";

	const char* A = NULL;
	const wchar_t* B = NULL;

	XStringT::IconvStringTranscoder tran;
	
	XStringT::StringAU8 str2;
	str2 = tran.stringFromANSI("123456 Hello World 测试文本");

	XStringT::StringAU8 str3;
	str3 = tran.stringFromANSI("ABCDEF");

//	str2.replace(5, 1, str3, 0);
//	str2.replace(5, 2, (XStringT::utf8*)"abcdef");

	//str3 = tran.stringFromStringW(std::wstring(L"123456 测试文本"));
	//str3 = tran.stringFromStringW(std::wstring(L"123456 測試文本"));
	//str3 = tran.stringFromStringW(std::wstring(L"123456 테스트 텍스트"));
	//str3 = tran.stringFromStringW(std::wstring(L"123456 テストテキスト"));
	//str3 = tran.stringFromStringW(std::wstring(L"123456 ทดสอบข้อความ"));
	//str3 = tran.stringFromStringW(std::wstring(L"123456 Kiểm tra văn bản"));

//	A = tran.stringToANSI(str3);
//	B = (const wchar_t*)tran.stringToUTF16(str3);
//	tran.deleteANSIBuffer(A);
//	tran.deleteUTF16Buffer((const XStringT::utf16*)B);

	XStringT::StringSwapperT<std::string> ss("123456 测试文本");
	ss += "123456 測試文本";
	ss += L"123456 테스트 텍스트";
	ss = ss + __XSTRINGT_STRING_UTF8A("123456 ทดสอบข้อความ");
	ss = ss + L"123456 テストテキスト";
	std::string rra = ss.astr();
	std::wstring rrw = ss.wstr();

	return 0;
}
