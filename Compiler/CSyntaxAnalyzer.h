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
	//таблица типов : ключ-название типа, значение-сам тип
	map<string, CType*> mapTypes = {
		{
			{"begin", new CType(Integer)},
			{"char", new CType(Char)},
			{"real", new CType(Real)}
		}
	};
	//таблица идентификаторов : ключ-идентификатор, значение-тип идентификатора
	map<string, CType*> mapIdentifiers;
	/// <summary>
	/// программа
	/// </summary>
	void Program();
	/// <summary>
	/// блок
	/// </summary>
	void Block();
	/// <summary>
	/// раздел типов
	/// </summary>
	void BlockTypes();
	/// <summary>
	/// определение типа
	/// </summary>
	void DefinitionType();
	/// <summary>
	/// раздел констант
	/// </summary>
	void BlockConstants();
	/// <summary>
	/// определение константы
	/// </summary>
	void DefinitionConstant();
	/// <summary>
	/// раздел переменных
	/// </summary>
	void BlockVariables();
	/// <summary>
	/// описание однотипных переменных
	/// </summary>
	void DefinitionVariables();
	/// <summary>
	/// тип
	/// </summary>
	/// <returns></returns>
	CType* Type();
	/// <summary>
	/// простой тип
	/// </summary>
	/// <returns></returns>
	CType* SimpleType();
	/// <summary>
	/// раздел операторов
	/// </summary>
	void BlockOperators();
	/// <summary>
	/// составной оператор
	/// </summary>
	void CompountOperator();
	/// <summary>
	/// оператор
	/// </summary>
	void _Operator();
	/// <summary>
	/// непомеченный оператор
	/// </summary>
	void UnlabeledOperator();
	/// <summary>
	/// простой оператор
	/// </summary>
	void SimpleOperator();
	/// <summary>
	/// сложный оператор
	/// </summary>
	void ComplexOperator();
	/// <summary>
	/// оператор присваивания
	/// </summary>
	void AssignOperator();
	/// <summary>
	/// переменная
	/// </summary>
	CType* Variable();
	/// <summary>
	/// выражение
	/// </summary>
	void Expression();
	/// <summary>
	/// имя
	/// </summary>
	string Name();
	/// <summary>
	/// имя файла
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
	/// <summary>
	/// Запускает синтаксический анализ
	/// </summary>
	void Analyze();
};
