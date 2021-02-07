#pragma once
#include "CVariant.h"
enum ETokenType
{
	Identifier,
	Value,
	Operator
};

enum EOperator 
{
	star, // *
	slash, // /
	equal, // =

};

class CToken {
	ETokenType type;
	/// <summary>
	/// Если тип токена - оператор, то тут будет тип оператора
	/// </summary>
	EOperator _operator;
	CVariant variant;
	CToken(ETokenType tokenType);
	CToken(ETokenType tokenType, EOperator operatorType);
};