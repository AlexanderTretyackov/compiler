#pragma once
#include <fstream>
#include <iostream>
#include "CType.h"
#include "CToken.h"
#include <stack>
#include <list>

using namespace types;

class CGenerator {
private:
	/// <summary>
	/// Стек меток для работы с оператором if
	/// </summary>
	stack<string> marksIf;
	/// <summary>
	/// Список меток начала оператора while (на условие)
	/// </summary>
	stack<string> marksWhileStart;
	/// <summary>
	/// Список меток конца оператора while
	/// </summary>
	stack<string> marksWhileEnd;
	ofstream outputIlFile;
	/// <summary>
	/// Отменена ли генерация
	/// </summary>
	bool isGenerationCancelled = false;
	int countMarks = 0;
public:
	CGenerator(string outputIlFileName);
	/// <summary>
	/// Добавляет определение переменной
	/// </summary>
	/// <param name="type">Тип переменной</param>
	/// <param name="varName">Название переменной</param>
	void DefineVariable(EType type, string variableName);

	void WriteDefaultInfo();
	/// <summary>
	/// Берет значение из стека и записывает в переменную
	/// </summary>
	/// <param name="variableName"></param>
	void WriteAssign(string variableName);
	void FinishWrite();
	/// <summary>
	/// Записывает в стек константу
	/// </summary>
	/// <param name="variant"></param>
	void WriteConst(EVariantType vairantType, string variantString);
	/// <summary>
	/// Записываем в стек значение переменной
	/// </summary>
	/// <param name="variableName"></param>
	void WriteVariableValueToStack(string variableName);
	/// <summary>
	/// Добавляет операцию
	/// </summary>
	void WriteOperation(EOperator operation);
	/// <summary>
	/// Отменяет генерацию кода
	/// </summary>
	void CancelGeneration();

	void WriteMarkIfStart();
	void WriteMarkElseStart();
	void WriteMarkIfEnd();

	void WriteMarkWhileStart();
	void WriteMarkWhileBodyStart();
	void WriteMarkWhileEnd();
};

