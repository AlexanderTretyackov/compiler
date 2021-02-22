#pragma once
#include <list>
#include "CToken.h"
#include <fstream>
#include <iostream>

using namespace std;

enum ELexicalError {
	invalidLiter,//встретилась литера не принадлежащая алфавиту языка
	invalidConstant,//
};

class CLexicalAnalyzer 
{
private:
	ifstream inputStream;
	string line = "";
	int numberCurrentLine, numberCurrentLiter;
	char currentChar;
	bool IsInteger(string numberString, int& value);
	bool IsDouble(string numberString, double& value);
	/// <summary>
	/// Проверяет является ли символ буквой
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	bool IsCharacter(char c);
	/// <summary>
	/// Проверяет является ли символ цифрой
	/// </summary>
	/// <param name="c"></param>
	/// <returns></returns>
	bool IsDigit(char c);
	char GetNextChar();
public:
	CLexicalAnalyzer(string fileName);
	/// <summary>
	/// Отдает следующий токен наружу
	/// </summary>
	/// <param name="token"></param>
	CToken* GetNextToken();
};