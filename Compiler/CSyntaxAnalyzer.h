#pragma once
#include <list>
#include <map>
#include "CLexicalAnalyzer.h"
#include <fstream>
#include <iostream>
#include "CType.h"

using namespace std;
using namespace types;

class CSyntaxAnalyzer
{
	CLexicalAnalyzer* lexicalAnalyzer;
	CTokenPtr currentTokenPtr;
	CType* typeInteger = new CType(EType::Integer);
	CType* typeChar = new CType(EType::Char);
	CType* typeReal = new CType(EType::Real);
	CType* typeBoolean = new CType(EType::Boolean);
	//таблица типов : ключ-название типа, значение-сам тип
	map<string, CType*> mapTypes = {
		{
			{"integer", typeInteger},
			{"char", typeChar},
			{"real", typeReal},
			{"boolean", typeBoolean}
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
	/// комбинированный тип
	/// </summary>
	/// <returns></returns>
	CType* CombinedType();
	/// <summary>
	/// раздел операторов
	/// </summary>
	void BlockOperators();
	/// <summary>
	/// условный оператор
	/// </summary>
	void IfOperator();
	/// <summary>
	/// оператор варианта
	/// </summary>
	void CaseOperator();
	/// <summary>
	/// оператор присоединения
	/// </summary>
	void WithOperator();
	/// <summary>
	/// цикл с предусловием
	/// </summary>
	void WhileOperator();
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
	CType* Expression();
	/// <summary>
	/// простое выражение
	/// </summary>
	/// <returns></returns>
	CType* SimpleExpression();
	/// <summary>
	/// слагаемое
	/// </summary>
	CType* Summand();
	/// <summary>
	/// множитель
	/// </summary>
	CType* Multiplier();
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
	CType* Constant();
	/// <summary>
	/// число без знака
	/// </summary>
	CType* NumberWithoutSign();
	/// <summary>
	/// элемент списка вариантов
	/// </summary>
	/// <returns></returns>
	CType* CaseListItem();
	/// <summary>
	/// список меток варианта
	/// </summary>
	/// <returns></returns>
	CType* CaseListLabels();
	/// <summary>
	/// метка варианта
	/// </summary>
	/// <returns></returns>
	CType* CaseLabel();
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
