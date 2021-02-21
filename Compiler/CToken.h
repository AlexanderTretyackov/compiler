#pragma once
#include "CVariant.h"

namespace compiler {
	enum EOperator
	{
		star, // *
		slash, // /
		equal, // =
		comma,// ,
		semicolon,// ;
		colon,// :
		point,// .
		arrow,// ^
		leftpar,// (
		rightpar,// )
		lbracket,// [
		rbracket,// ]
		flpar,// {
		frpar,// }
		later,// <
		greater,// >
		laterequal,// <=
		greaterequal,// >=
		latergreater,// <>
		plus,// +
		minus,// -
		lcomment,// (*
		rcomment,// *)
		assign,// :=
		twopoints,// ..
		_begin,
		_end,
		_var,
		_and,
		_array,
		_case,
		_const,
		_div,
		_do,	
		_file,
		_for,
		_if,
		_then,
		_else,
		_in,
		_mod,
		_not,
		_or,
		_program,
		_to,
		_while,
		_readln,
		_writeln
	};
}
enum ETokenType
{
	Identifier,
	Value,
	Operator
};

using namespace compiler;

class CToken {
public:
	ETokenType type;
	/// <summary>
	/// Если тип токена - оператор, то тут будет тип оператора
	/// </summary>
	EOperator _operator;
	CVariantPtr variantPtr;
	string identifier;
	/// <summary>
	/// для значения
	/// </summary>
	/// <param name="tokenType"></param>
	/// <param name="variant"></param>
	CToken(ETokenType tokenType, CVariant* variant);
	/// <summary>
	/// для оператора
	/// </summary>
	/// <param name="tokenType"></param>
	/// <param name="operatorType"></param>
	CToken(ETokenType tokenType, EOperator operatorType);
	CToken(ETokenType tokenType, string identifier);
	string ToString();
};

typedef std::unique_ptr<CToken> CTokenPtr;