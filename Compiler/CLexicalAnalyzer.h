#pragma once
#include <list>
#include "CError.h"
#include "Token.h"
#include <fstream>
#include <iostream>

using namespace std;

class CLexicalAnalyzer 
{
	/// <summary>
	/// Поток для чтения файла с исходным кодом
	/// </summary>
	ifstream inputStream;
	int currentLine, currentSymbol;
	CLexicalAnalyzer(string fileName);
	/// <summary>
	/// Список ошибок
	/// </summary>
	list<CError> errors;
	void AddError(CError error);
	/// <summary>
	/// Считывает новый токен
	/// </summary>
	/// <param name="token"></param>
	CToken ReadToken();
	/// <summary>
	/// Отдает следующий токен наружу
	/// </summary>
	/// <param name="token"></param>
	CToken* GetNextToken();
};