#pragma once
#include <fstream>
#include <iostream>
#include "CType.h"

using namespace types;

class CGenerator {
private:
	/// <summary>
	/// файл с il кодом
	/// </summary>
	ofstream outputIlFile;

public:
	CGenerator(string outputIlFileName);
	/// <summary>
	/// Добавляет определение переменной
	/// </summary>
	/// <param name="type">Тип переменной</param>
	/// <param name="varName">Название переменной</param>
	void DefineVariable(EType type, string variableName);

	void WriteDefaultInfo();
	void FinishWrite();
};

