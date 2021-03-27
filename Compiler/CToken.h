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
		_writeln,
		_eof,
		_type,
		_with
	};
}
enum ETokenType
{
	Identifier,
	Value,
	Operator,
	Eof
};

using namespace compiler;

class CToken {
public:
	ETokenType type;
	/// <summary>
	/// ≈сли тип токена - оператор, то тут будет тип оператора
	/// </summary>
	EOperator _operator;
	CVariantPtr variantPtr;
	string identifier;
	/// <summary>
	/// дл€ значени€
	/// </summary>
	/// <param name="tokenType"></param>
	/// <param name="variant"></param>
	CToken(ETokenType tokenType, CVariant* variant);
	/// <summary>
	/// дл€ оператора
	/// </summary>
	/// <param name="tokenType"></param>
	/// <param name="operatorType"></param>
	CToken(ETokenType tokenType, EOperator operatorType);
	CToken(ETokenType tokenType, string identifier);
	/// <summary>
	/// —равнивает токены на равенство
	/// </summary>
	/// <param name="otherToken"></param>
	/// <returns>true если эквивалентны, false - иначе</returns>
	bool IsEqual(CToken* otherToken);
	string ToString();
};

typedef std::unique_ptr<CToken> CTokenPtr;
