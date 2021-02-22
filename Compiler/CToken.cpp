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
//!!!!!!!!!!!!!!!!!!!!!!!!!!!! Метод предназначен пока только для сравнения при пропуске
bool CToken::IsEqual(CToken* otherToken) {
	CTokenPtr otherTokenPtr(otherToken);
	if (otherToken == nullptr)
		return false;
	if (otherTokenPtr->type == type)
	{
		//если токены явл. операторами, то типы операторов должны совпадать
		if (type == Operator)
			return _operator == otherToken->_operator;
	}
	return false;
}