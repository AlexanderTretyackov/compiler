#pragma once
#include <string>

using namespace std;

class CError {
	/// <summary>
	/// Номер строки с ошибкой
	/// </summary>
	int row;
	/// <summary>
	/// Номер символа в строке, на котором ошибка
	/// </summary>
	int symbol;
	/// <summary>
	/// Получить текст ошибки
	/// </summary>
	/// <returns></returns>
	virtual string ToString();
};
//map с ошибками