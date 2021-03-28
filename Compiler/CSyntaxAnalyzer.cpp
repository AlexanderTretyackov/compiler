#include "CSyntaxAnalyzer.h"
#include "CLexicalAnalyzer.h"
#include "CException.h"
#include <list>

using namespace types;

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
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _begin)
		BlockOperators();
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
	auto ident = Name();
	Accept(new CToken(Operator, compiler::equal));//=
	auto type = Type();
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
	while(currentTokenPtr->type == Identifier)
		DefinitionVariables();
}

void CSyntaxAnalyzer::DefinitionVariables()
{
	//список нужен для временного хранения идентификаторов, пока не дойдем до типа объявленных переменных
	list<string> listNewVariablesIdentifiers;
	//идентификатор новой переменной
	auto newVariableIdentifier = Name();
	//если переменная с таким идентификатором уже содержится в области видимости, то кидаем исключение
	if (mapIdentifiers.count(newVariableIdentifier) != 0)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Identifier already exist");
	//записываем в временный список новый идентификатор
	listNewVariablesIdentifiers.push_back(newVariableIdentifier);
	while(currentTokenPtr->type == Operator && currentTokenPtr->_operator == comma)//,
	{
		NextToken();
		newVariableIdentifier = Name();
		if (mapIdentifiers.count(newVariableIdentifier) != 0)
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Identifier already exist");
		//записываем в временный список новый идентификатор
		listNewVariablesIdentifiers.push_back(newVariableIdentifier);
		//записываем в таблицу идентификаторов новые идентификаторы пока без типа
		mapIdentifiers[newVariableIdentifier] = nullptr;
	}
	Accept(new CToken(Operator, colon));//:
	auto typeVariables = Type();
	Accept(new CToken(Operator, semicolon));//;
	//указываем тип у добавленных идентификаторов
	for (auto iterator = listNewVariablesIdentifiers.begin(); iterator != listNewVariablesIdentifiers.end(); iterator++) 
	{
		newVariableIdentifier = *iterator;
		mapIdentifiers[newVariableIdentifier] = typeVariables;
	}
}

CType* CSyntaxAnalyzer::Type()
{
	if (currentTokenPtr->type == Identifier && currentTokenPtr->identifier == "record")
		return CombinedType();
	return SimpleType();
}

CType* CSyntaxAnalyzer::SimpleType()
{
	if (currentTokenPtr->type == Identifier)
	{
		auto identifier = currentTokenPtr->identifier;
		//если тип есть, то возвращаем его, иначе кидаем исключение
		if (mapTypes.count(identifier) != 0)
		{
			NextToken();
			return mapTypes[identifier];
		}
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Type not defined");
	}
	throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected type identifier");
}

CType* CSyntaxAnalyzer::CombinedType()
{
	return nullptr;
}

void CSyntaxAnalyzer::BlockOperators()
{
	CompountOperator();
}

void CSyntaxAnalyzer::IfOperator()
{
	Accept(new CToken(Operator, _if));
	auto typeExpression = Expression();
	if(typeExpression != typeBoolean)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected boolean type expression");
	Accept(new CToken(Operator, _then));
	_Operator();
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _else)
	{
		NextToken();
		_Operator();
	}
}

void CSyntaxAnalyzer::CaseOperator()
{
	Accept(new CToken(Operator, _case));
	auto typeCaseExpression = Expression();
	//case поддерживает только типы integer и char
	if (typeCaseExpression != typeInteger && typeCaseExpression != typeChar)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected integer or char type expression");
	Accept(new CToken(Operator, _of));

	//если текущий токен константа integer или char, то разбираем как элемент списка вариантов
	if (currentTokenPtr->type == Value &&
		(currentTokenPtr->variantPtr->type == Integer ||
			currentTokenPtr->variantPtr->type == Char))
	{
		//если тип выражения case не совпадает с типом элемента списка вариантов
		if(typeCaseExpression != CaseListItem())
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Type of case expression doesn't match type label");
	}

	while(currentTokenPtr->type == Operator && 
		currentTokenPtr->_operator == semicolon)
	{
		NextToken();
		//если текущий токен константа integer или char, то разбираем как элемент списка вариантов
		if (currentTokenPtr->type == Value &&
			(currentTokenPtr->variantPtr->type == Integer ||
				currentTokenPtr->variantPtr->type == Char))
		{
			//если тип выражения case не совпадает с типом элемента списка вариантов
			if (typeCaseExpression != CaseListItem())
				throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Type of case expression doesn't match type label");
		}
	}

	//если case закончился
	if (currentTokenPtr->type == Operator && currentTokenPtr->_operator == _end)
	{
		NextToken();
		return;
	}

	throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected integer or char constant or end operator");
}

void CSyntaxAnalyzer::WithOperator()
{
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
	if (currentTokenPtr->type == Identifier)
		SimpleOperator();
	else
		ComplexOperator();
}

void CSyntaxAnalyzer::SimpleOperator()
{
	AssignOperator();
}

void CSyntaxAnalyzer::ComplexOperator()
{
	if (currentTokenPtr->type == Operator)
	{
		switch (currentTokenPtr->_operator)
		{
			case _begin: CompountOperator(); return;
			case _if: IfOperator(); return;
			case _case: CaseOperator(); return;
			case _with: WithOperator(); return;
		}
	}
}

void CSyntaxAnalyzer::AssignOperator()
{
	auto typeVariable = Variable();
	Accept(new CToken(Operator, assign));//:=
	auto typeExpression = Expression();
	if(typeVariable != typeExpression)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Types variable and expression doesn't match");
}

CType* CSyntaxAnalyzer::Variable()
{
	if (currentTokenPtr->type != Identifier)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
			"Expected variable");
	//если в таблице идентификаторов нет рассматриваемой переменной, то кидаем исключение
	if(mapIdentifiers.count(currentTokenPtr->identifier) == 0)
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Variable not defined");
	auto identifier = currentTokenPtr->identifier;
	NextToken();
	//возвращаем тип переменной
	return mapIdentifiers[identifier];
}

CType* CSyntaxAnalyzer::Expression()
{
	auto typeExpression = SimpleExpression();
	//если операция отношения
	if (currentTokenPtr->type == Operator &&
		(currentTokenPtr->_operator == later || //<
			currentTokenPtr->_operator == compiler::greater || //>
			currentTokenPtr->_operator == compiler::laterequal || //<=
			currentTokenPtr->_operator == compiler::greaterequal || //>=
			currentTokenPtr->_operator == compiler::latergreater)) //<>
	{
		NextToken();
		if(typeExpression != SimpleExpression())
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected another expression type");
		//возвращаем логический тип для выражения, т.к. был оператор сравнения
		return typeBoolean;
	}
	return typeExpression;
}

CType* CSyntaxAnalyzer::SimpleExpression()
{
	//сохраняем тип слагаемого
	auto typeSummand = Summand();
	while(currentTokenPtr->type == Operator && 
			(currentTokenPtr->_operator == compiler::plus || //+
			currentTokenPtr->_operator == compiler::minus || //-
			currentTokenPtr->_operator == compiler::_or)) //or
	{
		NextToken();
		//если типы слагаемых не совпадают, то кидаем исключение
		if (typeSummand != Summand())
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
				"Expected another type");
	}
	return typeSummand;
}

CType* CSyntaxAnalyzer::Summand()
{
	//сохраняем тип множителя
	auto typeMultiplier = Multiplier();

	while (currentTokenPtr->type == Operator &&
		(currentTokenPtr->_operator == compiler::star || //*
			currentTokenPtr->_operator == compiler::slash || ///
			currentTokenPtr->_operator == compiler::_div ||
			currentTokenPtr->_operator == compiler::_mod ||
			currentTokenPtr->_operator == compiler::_and))
	{
		NextToken();
		//если типы множителей не совпадают, то кидаем исключение
		if (typeMultiplier != Multiplier())
			throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected another type");
	}
	return typeMultiplier;
}

CType* CSyntaxAnalyzer::Multiplier()
{
	if (currentTokenPtr->type == Operator)
	{
		if (currentTokenPtr->_operator == leftpar)//(
		{
			NextToken();
			auto typeExpression = Expression();
			Accept(new CToken(Operator, rightpar));//)
			return typeExpression;
		}
		if (currentTokenPtr->_operator == _not)
		{
			NextToken();
			auto typeMultiplier = Expression();
			if(typeMultiplier != typeBoolean)
				throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
					"Expected boolean type multiplier");
			return typeMultiplier;
		}
	}

	if (currentTokenPtr->type == Value)
	{
		if (currentTokenPtr->variantPtr->type == Char)
		{
			NextToken();
			return typeChar;
		}
		return NumberWithoutSign();
	}

	if (currentTokenPtr->type == Identifier)
	{
		//если идентификатор есть, в таблице идентификаторов, то возвращаем его тип
		auto identifier = currentTokenPtr->identifier;
		if (mapIdentifiers.count(identifier) != 0)
		{
			NextToken();
			return mapIdentifiers[identifier];
		}
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Identifier not defined");
	}

	throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
		"Expected multiplier");
}

string CSyntaxAnalyzer::Name()
{
	if (currentTokenPtr->type == Identifier)
	{
		auto identifier = currentTokenPtr->identifier;
		//если такой идентификатор не объявлен
		if (mapIdentifiers.count(identifier) == 0)
		{
			NextToken();
			return identifier;
		}
		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Identifier with same name already defined");
	}
	throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), 
		"Expected identifier");
}

void CSyntaxAnalyzer::FileName()
{
	Name();
}

CType* CSyntaxAnalyzer::Constant()
{
	if (currentTokenPtr->type == Value)
	{
		auto typeVariant = currentTokenPtr->variantPtr->type;
		NextToken();
		switch (typeVariant)
		{
			case Char: return typeChar;
			case Integer: return typeInteger;
			case Real: return typeReal;
		}

		if (currentTokenPtr->type == Operator &&
			(currentTokenPtr->_operator == compiler::plus ||
				currentTokenPtr->_operator == compiler::minus))
		{
			NextToken();
			return NumberWithoutSign();
		}

		throw new SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
			"Expected constant");
	}
}

CType* CSyntaxAnalyzer::NumberWithoutSign()
{
	if (currentTokenPtr->type == Value)
	{
		auto variantType = currentTokenPtr->variantPtr->type;
		NextToken();
		if (variantType == Real)
			return typeReal;
		if (variantType == Integer)
			return typeInteger;
	}
	throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(), "Expected NumberWithoutSign");
}

CType* CSyntaxAnalyzer::CaseListItem()
{
	//список меток варианта
	auto typeLabels = CaseListLabels();
	Accept(new CToken(Operator, colon));
	if (currentTokenPtr->type == Value)
	{
		if (currentTokenPtr->variantPtr->type != Integer &&
			currentTokenPtr->variantPtr->type != Char)
			throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"Expected different type constant");
		return typeLabels;
	}
	_Operator();
	return typeLabels;
}

CType* CSyntaxAnalyzer::CaseListLabels()
{
	auto typeCaseLabel = CaseLabel();
	while (currentTokenPtr->type == Operator &&
		currentTokenPtr->_operator == comma)
	{
		NextToken();
		auto typeCaseLabelNext = CaseLabel();
		if(typeCaseLabel != typeCaseLabelNext)
			throw SyntaxException(lexicalAnalyzer->GetNumberLine(), lexicalAnalyzer->GetNumberChar(),
				"case labels have different types");
	}
	return typeCaseLabel;
}

CType* CSyntaxAnalyzer::CaseLabel()
{
	return Constant();
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