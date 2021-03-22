#include "CSyntaxAnalyzer.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"
#include <list>

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
	Accept(new CToken(Operator, semicolon));// ;
	Block();
}

/// <summary>
/// блок
/// </summary>
void CSyntaxAnalyzer::Block()
{
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _type)
		BlockTypes();
	if(currentTokenPtr->type == Operator && currentTokenPtr->_operator == _var)
		BlockVariables();
}

void CSyntaxAnalyzer::BlockTypes()
{
	Accept(new CToken(Operator, _type));
	DefinitionType();
	Accept(new CToken(Operator, semicolon));//;
	while(currentTokenPtr->type == Identifier)
		DefinitionType();
		Accept(new CToken(Operator, semicolon));//;
}

void CSyntaxAnalyzer::DefinitionType()
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

void CSyntaxAnalyzer::BlockVariables()
{
	Accept(new CToken(Operator, _var));
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _var)
	{
		while(currentTokenPtr->type == Identifier)
			DefinitionVariables();
	}
}

void CSyntaxAnalyzer::DefinitionVariables()
{
	//список нужен для временного хранения идентификаторов, пока не дойдем до типа объявленных переменных
	list<string> listNewVariablesIdentifiers;
	//идентификатор новой переменной
	auto newVariableIdentifier = Name();
	//если переменная с таким идентификатором уже содержится в области видимости, то кидаем исключение
	if (mapIdentifiers.count(newVariableIdentifier) != 0);
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Identifier already exist");
	//записываем в временный список новый идентификатор
	listNewVariablesIdentifiers.push_back(newVariableIdentifier);
	while(currentTokenPtr->type == Operator && currentTokenPtr->_operator == comma)//,
	{
		NextToken();
		newVariableIdentifier = Name();
		if (mapIdentifiers.count(newVariableIdentifier) != 0);
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Identifier already exist");
		//записываем в временный список новый идентификатор
		listNewVariablesIdentifiers.push_back(newVariableIdentifier);
		//записываем в таблицу идентификаторов новые идентификаторы пока без типа
		mapIdentifiers[newVariableIdentifier] = nullptr;
	}
	Accept(new CToken(Operator, colon));//:
	auto typeVariables = Type();
	//указываем тип у добавленных идентификаторов
	for (auto iterator = listNewVariablesIdentifiers.begin(); iterator != listNewVariablesIdentifiers.end(); iterator++) 
	{
		newVariableIdentifier = *iterator;
		mapIdentifiers[newVariableIdentifier] = typeVariables;
	}
}

CType* CSyntaxAnalyzer::Type()
{
	return SimpleType();
}

CType* CSyntaxAnalyzer::SimpleType()
{
	if (currentTokenPtr->type == Identifier)
	{
		if(currentTokenPtr->identifier == "integer")
			return new CType(Integer);
		if (currentTokenPtr->identifier == "real")
			return new CType(Real);
		if (currentTokenPtr->identifier == "char")
			return new CType(Char);
		if (currentTokenPtr->identifier == "string")
			return new CType(String);
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
			"Expected type identifier");
	}
}

void CSyntaxAnalyzer::BlockOperators()
{
	CompountOperator();
}

void CSyntaxAnalyzer::CompountOperator()
{
	Accept(new CToken(Operator, _begin));
	_Operator();
	while (currentTokenPtr->type == Operator && currentTokenPtr->_operator == semicolon)
	{
		NextToken();
		_Operator();
	}
	Accept(new CToken(Operator, _end));
}

void CSyntaxAnalyzer::_Operator()
{
	UnlabeledOperator();
}

void CSyntaxAnalyzer::UnlabeledOperator()
{

}

void CSyntaxAnalyzer::SimpleOperator()
{
	AssignOperator();
}

void CSyntaxAnalyzer::ComplexOperator()
{
}

void CSyntaxAnalyzer::AssignOperator()
{
	Variable();
	Accept(new CToken(Operator, assign));//:=
	Expression();
}

CType* CSyntaxAnalyzer::Variable()
{
	return new CType(Integer);
}

void CSyntaxAnalyzer::Expression()
{
}

string CSyntaxAnalyzer::Name()
{
	if (currentTokenPtr->type == Identifier)
		return currentTokenPtr->identifier;
	else
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
			"Expected identifier");
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