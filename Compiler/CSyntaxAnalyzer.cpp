#include "CSyntaxAnalyzer.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"

CSyntaxAnalyzer::CSyntaxAnalyzer(string fileName)
{
	lexicalAnalyzer = new CLexicalAnalyzer(fileName);
}

void CSyntaxAnalyzer::Analyze()
{
	NextToken();
	Program();
}

/// <summary>
/// программа
/// </summary>
void CSyntaxAnalyzer::Program()
{
	Accept(new CToken(Operator, _program));
	Name();
	Accept(new CToken(Operator, leftpar));// (
	FileName();
	while (currentTokenPtr->type == Operator && 
		currentTokenPtr->_operator == comma) // ,
	{
		NextToken();
		FileName();
	}
	Accept(new CToken(Operator, rightpar));// )
	Accept(new CToken(Operator, semicolon));// ;
	Block();
}

/// <summary>
/// блок
/// </summary>
void CSyntaxAnalyzer::Block()
{

}

/// <summary>
/// раздел констант
/// </summary>
void CSyntaxAnalyzer::BlockConstants()
{
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _const)
	{

	}
}

void CSyntaxAnalyzer::DefinitionConstant()
{
	Name();
	Accept(new CToken(Operator, compiler::equal));
	Constant();
}

void CSyntaxAnalyzer::Name()
{
	Accept(new CToken(Identifier, ""));
}

void CSyntaxAnalyzer::FileName()
{
	Name();
}

void CSyntaxAnalyzer::Constant()
{
	if (currentTokenPtr->type == Value)
	{
		if (currentTokenPtr->variantPtr->type == Char ||
			currentTokenPtr->variantPtr->type == String)
		{
			NextToken();
			return;
		}
		if (currentTokenPtr->type == Operator &&
			(currentTokenPtr->_operator == compiler::plus ||
				currentTokenPtr->_operator == compiler::minus))
		{
			NumberWithoutSign();
		}
	}
}

void CSyntaxAnalyzer::NumberWithoutSign()
{
	if (currentTokenPtr->type == Value &&
		(currentTokenPtr->variantPtr->type == Real ||
			currentTokenPtr->variantPtr->type == Integer))
		NextToken();
	else
		throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), "NumberWithoutSign");
}




void CSyntaxAnalyzer::Accept(CToken* targetToken)
{
	CTokenPtr targetTokenPtr = CTokenPtr(targetToken);
	if (targetTokenPtr == nullptr ||
		currentTokenPtr->type != targetTokenPtr->type ||
		currentTokenPtr->type == Operator && currentTokenPtr->_operator != targetTokenPtr->_operator)
	{
		throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), "Expected other type token");
	}
	NextToken();
}

void CSyntaxAnalyzer::NextToken()
{
	//ссчитываем новый токен
	auto nextToken = lexicalAnalyzer->GetNextToken();
	//очищаем память из под текущего токена, записываем туда новый
	currentTokenPtr.reset(nextToken);
}