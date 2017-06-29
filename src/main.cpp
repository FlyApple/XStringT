//============================================================================
// Name        : XStringT.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include "XStringT.h"

//
int main()
{
	XStringT::String str1(std::string("hello"));
	XStringT::String str2("hello");

	const char* A = str2.c_str();
	str1 = str1 + " " + "world";
	const char* B = str1.c_str();

	return 0;
}
