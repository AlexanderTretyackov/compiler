#include "CToken.h"

CToken::CToken(ETokenType tokenType, CVariant* _variant)
{
	type = tokenType;
	// reset выполняет удаление предыдущего объекта, на который ссылался указатель, и установку нового значения.
	variantPtr.reset(_variant);
}

CToken::CToken(ETokenType tokenType, EOperator operatorType)
{
	type = tokenType;
	_operator = operatorType;
}

CToken::CToken(ETokenType tokenType, string _identifier)
{
	type = tokenType;
	identifier = _identifier;
}

string CToken::ToString()
{
	switch (type)
	{
		case Identifier:
			return identifier;
		case Value:
			return variantPtr->ToString();
		case Operator:
			return to_string((int)_operator);
	}
	return "empty_token";
}