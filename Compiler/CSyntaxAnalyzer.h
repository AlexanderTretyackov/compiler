#pragma once
#include <list>
#include <map>
#include "CLexicalAnalyzer.h"
#include <fstream>
#include <iostream>
#include "CType.h"

using namespace std;

class CSyntaxAnalyzer
{
	CLexicalAnalyzer* lexicalAnalyzer;
	CTokenPtr currentTokenPtr;
	map<string, CType*> IdentifiersMap;
	/// <summary>
	/// программа
	/// </summary>
	void Program();
	/// <summary>
	/// блок
	/// </summary>
	void Block();
	/// <summary>
	/// раздел констант
	/// </summary>
	void BlockConstants();
	/// <summary>
	/// определение константы
	/// </summary>
	void DefinitionConstant();
	/// <summary>
	/// им€
	/// </summary>
	void Name();
	/// <summary>
	/// им€ файла
	/// </summary>
	void FileName();
	/// <summary>
	/// константа
	/// </summary>
	void Constant();
	/// <summary>
	/// число без знака
	/// </summary>
	void NumberWithoutSign();
	/// <summary>
	/// ѕровер€ет соответсвие текущего токена целевому
	/// </summary>
	/// <param name="token"></param>
	void Accept(CToken* targetToken);
	/// <summary>
	/// ¬ текущий токен записывает новый
	/// </summary>
	void NextToken();
public:
	CSyntaxAnalyzer(string fileName);
	/// <summary>
	/// «апускает синтаксический анализ
	/// </summary>
	void Analyze();
};
