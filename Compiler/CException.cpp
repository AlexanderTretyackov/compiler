#include "CException.h"

LexicalException::LexicalException(int _numberLine, int _numberLiter, const char* errorDescription)
	: exception(errorDescription)
{
	numberLine = _numberLine;
	numberLiter = _numberLiter;
}

string LexicalException::ToString()
{
	return "LexicalError: position: " +
		to_string(numberLine) + "," + to_string(numberLiter) +
		".Description: " + exception::what();
}

SyntaxException::SyntaxException(int _numberLine, int _numberLiter, const char* errorDescription)
	: exception(errorDescription)
{
	numberLine = _numberLine;
	numberLiter = _numberLiter;
}

string SyntaxException::ToString()
{
	return "SyntaxError: position: " +
		to_string(numberLine) + "," + to_string(numberLiter) +
		".Description: " + exception::what();
}