#pragma once
#include <list>
#include <map>
#include "CLexicalAnalyzer.h"
#include <fstream>
#include <iostream>
#include "CType.h"

using namespace std;
using namespace types;

enum ExceptionType {
	Lexical,
	Syntax,
	Semantic
};

const string StrExceptionsTypes[3] = { "Lexical", "Syntax", "Semantic" };

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
	map<string, CType*> mapIdentifiers = { {"true", typeBoolean}, {"false", typeBoolean } };
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
	void DefinitionType(list<EOperator> followers);
	/// <summary>
	/// раздел переменных
	/// </summary>
	void BlockVariables();
	/// <summary>
	/// описание однотипных переменных
	/// </summary>
	void DefinitionVariables(list<EOperator> followers);
	/// <summary>
	/// тип
	/// </summary>
	/// <returns></returns>
	CType* Type(list<EOperator> followers);
	/// <summary>
	/// простой тип
	/// </summary>
	/// <returns></returns>
	CType* SimpleType();
	/// <summary>
	/// комбинированный тип
	/// </summary>
	/// <returns></returns>
	CType* CombinedType(list<EOperator> followers);
	/// <summary>
	/// список полей
	/// </summary>
	map<string, CType*> ListFields(list<EOperator> followers);
	/// <summary>
	/// секция записи
	/// </summary>
	/// <param name="mapIdentifiersRecord">Таблица идентификаторов для записи</param>
	/// <returns></returns>
	void SectionRecord(map<string, CType*>& mapIdentifiersRecord, list<EOperator> followers);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="mapIdentifiersRecord">Таблица идентификаторов для записи</param>
	/// <returns></returns>
	string NameField();
	/// <summary>
	/// раздел операторов
	/// </summary>
	void BlockOperators();
	/// <summary>
	/// условный оператор
	/// </summary>
	void IfOperator(list<EOperator> followers);
	/// <summary>
	/// оператор варианта
	/// </summary>
	void CaseOperator(list<EOperator> followers);
	/// <summary>
	/// оператор присоединения
	/// </summary>
	void WithOperator();
	/// <summary>
	/// цикл с предусловием
	/// </summary>
	void WhileOperator(list<EOperator> followers);
	/// <summary>
	/// составной оператор
	/// </summary>
	void CompountOperator(list<EOperator> followers);
	/// <summary>
	/// оператор
	/// </summary>
	void _Operator(list<EOperator> followers);
	/// <summary>
	/// непомеченный оператор
	/// </summary>
	void UnlabeledOperator(list<EOperator> followers);
	/// <summary>
	/// простой оператор
	/// </summary>
	void SimpleOperator(list<EOperator> followers);
	/// <summary>
	/// сложный оператор
	/// </summary>
	void ComplexOperator(list<EOperator> followers);
	/// <summary>
	/// оператор присваивания
	/// </summary>
	void AssignOperator(list<EOperator> followers);
	/// <summary>
	/// переменная
	/// </summary>
	CType* Variable(list<EOperator> followers);
	/// <summary>
	/// компонента переменной
	/// </summary>
	/// <returns></returns>
	CType* VariableComponent();
	/// <summary>
	/// обозначение поля
	/// </summary>
	/// <returns></returns>
	CType* FieldDesignation();
	/// <summary>
	/// выражение
	/// </summary>
	CType* Expression(list<EOperator> followers);
	/// <summary>
	/// простое выражение
	/// </summary>
	/// <returns></returns>
	CType* SimpleExpression(list<EOperator> followers);
	/// <summary>
	/// слагаемое
	/// </summary>
	CType* Summand(list<EOperator> followers);
	/// <summary>
	/// множитель
	/// </summary>
	CType* Multiplier(list<EOperator> followers);
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
	CType* Constant(list<EOperator> followers);
	/// <summary>
	/// число без знака
	/// </summary>
	CType* NumberWithoutSign();
	/// <summary>
	/// элемент списка вариантов
	/// </summary>
	/// <returns></returns>
	CType* CaseListItem(list<EOperator> followers);
	/// <summary>
	/// список меток варианта
	/// </summary>
	/// <returns></returns>
	CType* CaseListLabels(list<EOperator> followers);
	/// <summary>
	/// метка варианта
	/// </summary>
	/// <returns></returns>
	CType* CaseLabel(list<EOperator> followers);
	/// <summary>
	/// Проверяет соответсвие текущего токена целевому
	/// </summary>
	/// <param name="token"></param>
	void Accept(CToken* targetToken);
	/// <summary>
	/// В текущий токен записывает новый
	/// </summary>
	void NextToken();
	/// <summary>
	/// Проверяет наличие оператора среди переданных
	/// </summary>
	/// <param name="operators">Операторы, среди которых идет поиск</param>
	/// <param name="findingOperator">Искомый оператор</param>
	/// <returns></returns>
	bool IsBelong(EOperator findingOperator, list<EOperator> operators);
	bool SkipToOperators(list<EOperator> operators, list<EOperator> followerOperators);
	bool SkipToOperators(list<EOperator> operators);
	void PrintExceptionMessage(ExceptionType excType,int line, int liter, string exceptionMessage);
public:
	CSyntaxAnalyzer(string fileName);
	/// <summary>
	/// Запускает синтаксический анализ
	/// </summary>
	void Analyze();
};
