#pragma once

#include <iostream>
#include <string>
using namespace std;

class LexicalException : exception
{
	int numberLine, numberLiter;
public:
	LexicalException(int _numberLine, int _numberLiter, const char* errorDescription);
	string ToString();
};