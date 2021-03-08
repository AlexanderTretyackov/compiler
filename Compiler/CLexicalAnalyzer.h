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
	int GetNumberLine();
	int GetNumberChar();
	CLexicalAnalyzer(string fileName);
	/// <summary>
	/// Отдает следующий токен наружу
	/// </summary>
	/// <param name="token"></param>
	CToken* GetNextToken();
	/// <summary>
	/// Пропускает все токены до указанного в параметрах. 
	/// Если после вызова этого метода вызвать GetNextToken, 
	/// то вернется следующий после пропускаемого.
	/// </summary>
	/// <param name="token">Токен, до которого нужно пропускать</param>
	/// <returns>true - если успешно пропущены токены до нужного, false - если не встретили нужного токена и дошли до конца файла</returns>
	bool SkipToToken(CToken* token);
};