#include <fstream>
#include <iostream>
#include <map>
#include "CLexicalAnalyzer.h"
#include "CException.h"

using namespace std;

const map<int, string> errorsMap =
{
	{ invalidLiter, "Invalid liter."},
	{ invalidConstant, "Invalid constant."}
};


const map<string, int> keywordsMap =
	{
		{"begin", compiler::_begin},
		{"end", compiler::_end},
		{"var", compiler::_var},
		{"and", compiler::_and},
		{"array", compiler::_array},
		{"case", compiler::_case},
		{"const", compiler::_const},
		{"div", compiler::_div},
		{"do", compiler::_do},
		{"file", compiler::_file},
		{"for", compiler::_for},
		{"if", compiler::_if},
		{"then", compiler::_then},
		{"else", compiler::_else},
		{"in", compiler::_in},
		{"mod", compiler::_mod},
		{"not", compiler::_not},
		{"or", compiler::_or},
		{"program", compiler::_program},
		{"to", compiler::_to},
		{"while", compiler::_while},
		{"readln", compiler::_readln},
		{"writeln", compiler::_writeln},
		{"type", compiler::_type},
		{"with", compiler::_with},
		{"of", compiler::_of}
	};

int CLexicalAnalyzer::GetNumberLine()
{
	return numberCurrentLine;
}

int CLexicalAnalyzer::GetNumberChar()
{
	return numberCurrentLiter;
}

bool CLexicalAnalyzer::IsInteger(string numberString, int& value)
{
	try {
		value = stoi(numberString);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CLexicalAnalyzer::IsDouble(string numberString, double& value)
{
	try {
		value = stod(numberString);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool CLexicalAnalyzer::IsCharacter(char c)
{
	return 'A' <= c && c <= 'Z' ||
		'a' <= c && c <= 'z';
}

bool CLexicalAnalyzer::IsDigit(char c)
{
	return '0' <= c && c <= '9';
}

CLexicalAnalyzer::CLexicalAnalyzer(string fileName) 
{
	//открываем файл с исходным кодом на чтение
	inputStream.open(fileName);
	numberCurrentLine = 0;
	numberCurrentLiter = 0;
}

char CLexicalAnalyzer::GetNextChar()
{
	//если текущая строка пустая или уже обработана
	if (line == "" || numberCurrentLiter == line.length())
	{
		//если не конец файла, то считываем новую строку
		if (!inputStream.eof())
		{
			getline(inputStream, line);
			line += '\n';
			//inputStream >> line;
			numberCurrentLine++;
			numberCurrentLiter = 0;
		}
		else
			return EOF;
	}

	if (line.length() > 0)
	{
		return line[numberCurrentLiter++];
	}
}

void CLexicalAnalyzer::SkipWhitespaces()
{
	while(currentChar == ' ' || currentChar == '\t' ||
		currentChar == '\0' || currentChar == '\n')
		currentChar = GetNextChar();
}

CToken* CLexicalAnalyzer::GetNextToken()
{
	//пропускаем пробельные символы
	SkipWhitespaces();

	//пропускаем комментарии
	if (currentChar == '{')
	{
		while (currentChar != '}')
			currentChar = GetNextChar();
		currentChar = GetNextChar();
	}
	if (currentChar == '(')
	{
		currentChar = GetNextChar();
		if (currentChar == '*')
		{
			currentChar = GetNextChar();
			char prevChar = '0';
			while (currentChar != ')' && prevChar != '*')
			{
				prevChar = currentChar;
				currentChar = GetNextChar();
			}
			currentChar = GetNextChar();
		}
		else
			return new CToken(Operator, leftpar);
	}
	SkipWhitespaces();
			
	switch (currentChar)
	{
	case ')':
		currentChar = GetNextChar();
		return new CToken(Operator, rightpar);
	case '/':
		currentChar = GetNextChar();
		if (currentChar == '/')
		{
			while (currentChar != '\n')
				currentChar = GetNextChar();
			currentChar = GetNextChar();
		}
		else
			return new CToken(Operator, slash);
	case '+':
		currentChar = GetNextChar();
		return new CToken(Operator, compiler::plus);
	case '-':
		currentChar = GetNextChar();
		return new CToken(Operator, compiler::minus);
	case '*':
		currentChar = GetNextChar();
		return new CToken(Operator, star);
	case '=':
		currentChar = GetNextChar();
		return new CToken(Operator, compiler::equal);
	case '.':
		currentChar = GetNextChar();
		if (currentChar == '.')
		{
			currentChar = GetNextChar();
			return new CToken(Operator, twopoints);
		}
		return new CToken(Operator, point);
	case ',':
		currentChar = GetNextChar();
		return new CToken(Operator, comma);
	case '<':
		currentChar = GetNextChar();
		if (currentChar == '=')
		{
			currentChar = GetNextChar();
			return new CToken(Operator, laterequal);
		}
		if (currentChar == '>')
		{
			currentChar = GetNextChar();
			return new CToken(Operator, latergreater);
		}
		currentChar = GetNextChar();
		return new CToken(Operator, later);
	case '>':
		currentChar = GetNextChar();
		if (currentChar == '=')
		{
			currentChar = GetNextChar();
			return new CToken(Operator, greaterequal);
		}
		currentChar = GetNextChar();
		return new CToken(Operator, compiler::greater);
	case ':':
		currentChar = GetNextChar();
		if (currentChar == '=')
		{
			currentChar = GetNextChar();
			return new CToken(Operator, assign);
		}
		return new CToken(Operator, colon);
	case ';':
		currentChar = GetNextChar();
		return new CToken(Operator, semicolon);
	case '^':
		currentChar = GetNextChar();
		return new CToken(Operator, arrow);
	case '[':
		currentChar = GetNextChar();
		return new CToken(Operator, lbracket);
	case ']':
		currentChar = GetNextChar();
		return new CToken(Operator, rbracket);
	case '\'':
	{
		string str = "";
		currentChar = GetNextChar();
		while (currentChar != '\'')
		{
			str += currentChar;
			currentChar = GetNextChar();
		}
		currentChar = GetNextChar();
		return new CToken(Value, new CStringVariant(str));
	}
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		string numberString = "";
		int beginNumberCurrentLine = numberCurrentLine, 
			beginNumberCurrentLiter = numberCurrentLiter;
		int countPoints = 0;
		while (currentChar >= '0' && currentChar <= '9' || currentChar == '.')
		{					
			if (currentChar == '.')
				countPoints++;
			numberString += currentChar;
			currentChar = GetNextChar();
		}

		if (countPoints > 0)
		{
			if (countPoints == 1)
			{
				double doubleValue;
				if (IsDouble(numberString, doubleValue))
					return new CToken(Value, new CRealVariant(doubleValue));
			}
		}
		else
		{
			int intValue;
			if (IsInteger(numberString, intValue))
				return new CToken(Value, new CIntegerVariant(intValue));
		}
			
		//если не удалось сконвертировать ни в целое ни в вещественное
		throw LexicalException(beginNumberCurrentLine, beginNumberCurrentLiter,
			errorsMap.find(invalidConstant)->second.c_str());
		break;
	}

	if (IsCharacter(currentChar))
	{
		string identifier = "";
		while (IsCharacter(currentChar) ||
			currentChar == '_' ||
			IsDigit(currentChar))
		{
			identifier += currentChar;
			currentChar = GetNextChar();
		}
		const auto foundKeyword = keywordsMap.find(identifier);
		//если идентификатор является ключевым словом
		if (foundKeyword != keywordsMap.cend())
			return new CToken(Operator, (EOperator)foundKeyword->second);
		return new CToken(Identifier, identifier);
	}

	if(currentChar == EOF)
		return new CToken(Eof, _eof);

	int _line = numberCurrentLine,
		_liter = numberCurrentLiter;
	currentChar = GetNextChar();
	//если встретили литеру не из алфавита
	throw LexicalException(_line, _liter,
		errorsMap.find(invalidLiter)->second.c_str());
}

bool CLexicalAnalyzer::SkipToToken(CToken* token)
{
	CTokenPtr currentTokenPtr(nullptr);
	while (!token->IsEqual(currentTokenPtr.release()))
	{
		try {
			currentTokenPtr.reset(GetNextToken());
			//если дошли до конца файла
			if (currentTokenPtr->type == Eof)
				return false;
			//cout << currentTokenPtr->ToString();
		}
		catch (LexicalException& e)
		{
			//cout << e.ToString() << '\n';
		}
	} 
	return true;
}