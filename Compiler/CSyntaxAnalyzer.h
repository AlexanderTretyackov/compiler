#pragma once
#include <list>
#include "CLexicalAnalyzer.h"
#include <fstream>
#include <iostream>

using namespace std;

class CSyntaxAnalyzer
{
	CLexicalAnalyzer* lexicalAnalyzer;
	CTokenPtr currentTokenPtr;
	/// <summary>
	/// Раздел объявление программы
	/// </summary>
	void Program();
	/// <summary>
	/// Проверяет соответсвие текущего токена целевому
	/// </summary>
	/// <param name="token"></param>
	void Accept(CToken* targetToken);
	/// <summary>
	/// В текущий токен записывает новый
	/// </summary>
	void NextToken();
public:
	CSyntaxAnalyzer(string fileName);
};
